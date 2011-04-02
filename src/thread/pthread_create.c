#include "pthread_impl.h"

#ifdef __pthread_unwind_next
#undef __pthread_unwind_next
#define __pthread_unwind_next __pthread_unwind_next_3
#endif

void __pthread_unwind_next(struct __ptcb *cb)
{
	int i, j, not_finished;
	pthread_t self;

	if (cb->__next) longjmp((void *)cb->__next->__jb, 1);

	self = pthread_self();

	LOCK(&self->exitlock);

	not_finished = self->tsd_used;
	for (j=0; not_finished && j<PTHREAD_DESTRUCTOR_ITERATIONS; j++) {
		not_finished = 0;
		for (i=0; i<PTHREAD_KEYS_MAX; i++) {
			if (self->tsd[i] && libc.tsd_keys[i]) {
				void *tmp = self->tsd[i];
				self->tsd[i] = 0;
				libc.tsd_keys[i](tmp);
				not_finished = 1;
			}
		}
	}

	/* Mark this thread dead before decrementing count */
	self->dead = 1;

	if (!a_fetch_add(&libc.threads_minus_1, -1))
		exit(0);

	if (self->detached && self->map_base) {
		syscall(__NR_rt_sigprocmask, SIG_BLOCK, (long)(uint64_t[1]){-1},0,8);
		__unmapself(self->map_base, self->map_size);
	}

	syscall(SYS_exit, 0);
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
	if (!self->cancel) {
		if (si->si_code == SI_TIMER && libc.sigtimer)
			libc.sigtimer(sig, si, ctx);
		return;
	}
	if (self->canceldisable) return;
	if (self->cancelasync || (self->cancelpoint==1 && PC_AT_SYS(ctx)))
		docancel(self);
}

static void cancelpt(int x)
{
	struct pthread *self = __pthread_self();
	if (self->canceldisable) return;
	if ((self->cancelpoint+=x)==1 && x>=0 && self->cancel)
		docancel(self);
}

/* "rsyscall" is a mechanism by which a thread can synchronously force all
 * other threads to perform an arbitrary syscall. It is necessary to work
 * around the non-conformant implementation of setuid() et al on Linux,
 * which affect only the calling thread and not the whole process. This
 * implementation performs some tricks with signal delivery to work around
 * the fact that it does not keep any list of threads in userspace. */

static struct {
	volatile int lock, hold, blocks, cnt;
	unsigned long arg[6];
	int nr;
	int err;
} rs;

static void rsyscall_handler(int sig, siginfo_t *si, void *ctx)
{
	struct pthread *self = __pthread_self();

	if (si->si_code > 0 || si->si_pid != self->pid ||
		rs.cnt == libc.threads_minus_1) return;

	/* Threads which have already decremented themselves from the
	 * thread count must not increment rs.cnt or otherwise act. */
	if (self->dead) {
		__wait(&rs.hold, 0, 1, 1);
		return;
	}

	if (syscall(rs.nr, rs.arg[0], rs.arg[1], rs.arg[2],
		rs.arg[3], rs.arg[4], rs.arg[5]) < 0 && !rs.err) rs.err=errno;

	a_inc(&rs.cnt);
	__wake(&rs.cnt, 1, 1);
	while(rs.hold)
		__wait(&rs.hold, 0, 1, 1);
	a_dec(&rs.cnt);
	if (!rs.cnt) __wake(&rs.cnt, 1, 1);
}

static int rsyscall(int nr, long a, long b, long c, long d, long e, long f)
{
	int i, ret;
	sigset_t set = { 0 };
	struct pthread *self = __pthread_self();
	sigaddset(&set, SIGSYSCALL);

	LOCK(&rs.lock);
	while ((i=rs.blocks))
		__wait(&rs.blocks, 0, i, 1);

	__libc_sigprocmask(SIG_BLOCK, &set, 0);

	rs.nr = nr;
	rs.arg[0] = a; rs.arg[1] = b;
	rs.arg[2] = c; rs.arg[3] = d;
	rs.arg[4] = d; rs.arg[5] = f;
	rs.hold = 1;
	rs.err = 0;
	rs.cnt = 0;

	/* Dispatch signals until all threads respond */
	for (i=libc.threads_minus_1; i; i--)
		sigqueue(self->pid, SIGSYSCALL, (union sigval){0});
	while ((i=rs.cnt) < libc.threads_minus_1) {
		sigqueue(self->pid, SIGSYSCALL, (union sigval){0});
		__wait(&rs.cnt, 0, i, 1);
	}

	/* Handle any lingering signals with no-op */
	__libc_sigprocmask(SIG_UNBLOCK, &set, 0);

	/* Resume other threads' signal handlers and wait for them */
	rs.hold = 0;
	__wake(&rs.hold, -1, 0);
	while((i=rs.cnt)) __wait(&rs.cnt, 0, i, 1);

	if (rs.err) errno = rs.err, ret = -1;
	else ret = syscall(nr, a, b, c, d, e, f);

	UNLOCK(&rs.lock);
	return ret;
}

static void init_threads()
{
	struct sigaction sa = { .sa_flags = SA_SIGINFO | SA_RESTART };
	libc.lock = __lock;
	libc.lockfile = __lockfile;
	libc.cancelpt = cancelpt;
	libc.rsyscall = rsyscall;
	sa.sa_sigaction = cancel_handler;
	__libc_sigaction(SIGCANCEL, &sa, 0);
	sigaddset(&sa.sa_mask, SIGSYSCALL);
	sigaddset(&sa.sa_mask, SIGCANCEL);
	sa.sa_sigaction = rsyscall_handler;
	__libc_sigaction(SIGSYSCALL, &sa, 0);
	sigprocmask(SIG_UNBLOCK, &sa.sa_mask, 0);
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
	static const pthread_attr_t default_attr;

	if (!self) return errno = ENOSYS;
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

	/* We must synchronize new thread creation with rsyscall
	 * delivery. This looks to be the least expensive way: */
	a_inc(&rs.blocks);
	while (rs.lock) __wait(&rs.lock, 0, 1, 1);

	a_inc(&libc.threads_minus_1);
	ret = __uniclone(stack, start, new);

	a_dec(&rs.blocks);
	if (rs.lock) __wake(&rs.blocks, 1, 1);

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
