#include "pthread_impl.h"

static void dummy_0()
{
}
weak_alias(dummy_0, __rsyscall_lock);
weak_alias(dummy_0, __rsyscall_unlock);

static void dummy_1(pthread_t self)
{
}
weak_alias(dummy_1, __pthread_tsd_run_dtors);
weak_alias(dummy_1, __sigtimer_handler);

#ifdef __pthread_unwind_next
#undef __pthread_unwind_next
#define __pthread_unwind_next __pthread_unwind_next_3
#endif

void __pthread_unwind_next(struct __ptcb *cb)
{
	pthread_t self;

	if (cb->__next) longjmp((void *)cb->__next->__jb, 1);

	self = pthread_self();

	LOCK(&self->exitlock);

	__pthread_tsd_run_dtors(self);

	/* Mark this thread dead before decrementing count */
	self->dead = 1;

	if (!a_fetch_add(&libc.threads_minus_1, -1))
		exit(0);

	if (self->detached && self->map_base) {
		__syscall(SYS_rt_sigprocmask, SIG_BLOCK, (long)(uint64_t[1]){-1},0,8);
		__unmapself(self->map_base, self->map_size);
	}

	__syscall(SYS_exit, 0);
}

static void docancel(struct pthread *self)
{
	struct __ptcb cb = { .__next = self->cancelbuf };
	self->canceldisable = 1;
	self->cancelasync = 0;
	__pthread_unwind_next(&cb);
}

static void cancel_handler(int sig, siginfo_t *si, void *ctx)
{
	struct pthread *self = __pthread_self();
	if (si->si_code == SI_TIMER) __sigtimer_handler(self);
	if (self->cancel && !self->canceldisable &&
	    (self->cancelasync || (self->cancelpoint==1 && PC_AT_SYS(ctx))))
		docancel(self);
}

static void cancelpt(int x)
{
	struct pthread *self = __pthread_self();
	switch (x) {
	case 1:
		self->cancelpoint++;
	case 0:
		if (self->cancel && self->cancelpoint==1 && !self->canceldisable)
			docancel(self);
		break;
	case -1:
		self->cancelpoint--;
		break;
	default:
		self->canceldisable += x;
	}
}

static void init_threads()
{
	struct sigaction sa = { .sa_flags = SA_SIGINFO | SA_RESTART };
	libc.lock = __lock;
	libc.lockfile = __lockfile;
	libc.cancelpt = cancelpt;

	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = cancel_handler;
	__libc_sigaction(SIGCANCEL, &sa, 0);

	sigaddset(&sa.sa_mask, SIGSYSCALL);
	sigaddset(&sa.sa_mask, SIGCANCEL);
	__libc_sigprocmask(SIG_UNBLOCK, &sa.sa_mask, 0);
}

static int start(void *p)
{
	struct pthread *self = p;
	if (self->unblock_cancel) {
		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set, SIGCANCEL);
		__libc_sigprocmask(SIG_UNBLOCK, &set, 0);
	}
	pthread_exit(self->start(self->start_arg));
	return 0;
}

int __uniclone(void *, int (*)(), void *);

#define ROUND(x) (((x)+PAGE_SIZE-1)&-PAGE_SIZE)

/* pthread_key_create.c overrides this */
static const size_t dummy = 0;
weak_alias(dummy, __pthread_tsd_size);

int pthread_create(pthread_t *res, const pthread_attr_t *attr, void *(*entry)(void *), void *arg)
{
	static int init;
	int ret;
	size_t size, guard;
	struct pthread *self = pthread_self(), *new;
	unsigned char *map, *stack, *tsd;
	const pthread_attr_t default_attr = { 0 };

	if (!self) return ENOSYS;
	if (!init && ++init) init_threads();

	if (!attr) attr = &default_attr;
	guard = ROUND(attr->_a_guardsize + DEFAULT_GUARD_SIZE);
	size = guard + ROUND(attr->_a_stacksize + DEFAULT_STACK_SIZE);
	size += __pthread_tsd_size;
	map = mmap(0, size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANON, -1, 0);
	if (!map) return EAGAIN;
	if (guard) mprotect(map, guard, PROT_NONE);

	tsd = map + size - __pthread_tsd_size;
	new = (void *)(tsd - sizeof *new - PAGE_SIZE%sizeof *new);
	new->map_base = map;
	new->map_size = size;
	new->pid = self->pid;
	new->errno_ptr = &new->errno_val;
	new->start = entry;
	new->start_arg = arg;
	new->self = new;
	new->tsd = (void *)tsd;
	new->detached = attr->_a_detach;
	new->attr = *attr;
	new->unblock_cancel = self->cancel;
	new->result = PTHREAD_CANCELED;
	memcpy(new->tlsdesc, self->tlsdesc, sizeof new->tlsdesc);
	new->tlsdesc[1] = (uintptr_t)new;
	stack = (void *)((uintptr_t)new-1 & ~(uintptr_t)15);

	__rsyscall_lock();

	a_inc(&libc.threads_minus_1);
	ret = __uniclone(stack, start, new);

	__rsyscall_unlock();

	if (ret < 0) {
		a_dec(&libc.threads_minus_1);
		munmap(map, size);
		return EAGAIN;
	}
	*res = new;
	return 0;
}

void pthread_exit(void *result)
{
	struct pthread *self = pthread_self();
	self->result = result;
	docancel(self);
}
