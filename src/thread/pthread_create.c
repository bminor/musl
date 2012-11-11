#include "pthread_impl.h"
#include "stdio_impl.h"
#include <sys/mman.h>

static void dummy_0()
{
}
weak_alias(dummy_0, __acquire_ptc);
weak_alias(dummy_0, __release_ptc);
weak_alias(dummy_0, __pthread_tsd_run_dtors);

_Noreturn void pthread_exit(void *result)
{
	pthread_t self = pthread_self();
	int n;

	self->result = result;

	while (self->cancelbuf) {
		void (*f)(void *) = self->cancelbuf->__f;
		void *x = self->cancelbuf->__x;
		self->cancelbuf = self->cancelbuf->__next;
		f(x);
	}

	__pthread_tsd_run_dtors();

	__lock(self->exitlock);

	/* Mark this thread dead before decrementing count */
	__lock(self->killlock);
	self->dead = 1;
	__unlock(self->killlock);

	do n = libc.threads_minus_1;
	while (n && a_cas(&libc.threads_minus_1, n, n-1)!=n);
	if (!n) exit(0);

	if (self->detached && self->map_base) {
		if (self->detached == 2)
			__syscall(SYS_set_tid_address, 0);
		__syscall(SYS_rt_sigprocmask, SIG_BLOCK,
			SIGALL_SET, 0, __SYSCALL_SSLEN);
		__unmapself(self->map_base, self->map_size);
	}

	for (;;) __syscall(SYS_exit, 0);
}

void __do_cleanup_push(struct __ptcb *cb)
{
	struct pthread *self = pthread_self();
	cb->__next = self->cancelbuf;
	self->cancelbuf = cb;
}

void __do_cleanup_pop(struct __ptcb *cb)
{
	__pthread_self()->cancelbuf = cb->__next;
}

static int start(void *p)
{
	pthread_t self = p;
	if (self->startlock[0]) {
		__wait(self->startlock, 0, 1, 1);
		if (self->startlock[0]) {
			self->detached = 2;
			pthread_exit(0);
		}
		__syscall(SYS_rt_sigprocmask, SIG_SETMASK,
			self->sigmask, 0, __SYSCALL_SSLEN);
	}
	if (self->unblock_cancel)
		__syscall(SYS_rt_sigprocmask, SIG_UNBLOCK,
			SIGPT_SET, 0, __SYSCALL_SSLEN);
	pthread_exit(self->start(self->start_arg));
	return 0;
}

#define ROUND(x) (((x)+PAGE_SIZE-1)&-PAGE_SIZE)

/* pthread_key_create.c overrides this */
static const size_t dummy = 0;
weak_alias(dummy, __pthread_tsd_size);

static FILE *const dummy_file = 0;
weak_alias(dummy_file, __stdin_used);
weak_alias(dummy_file, __stdout_used);
weak_alias(dummy_file, __stderr_used);

static void init_file_lock(FILE *f)
{
	if (f && f->lock<0) f->lock = 0;
}

void *__copy_tls(unsigned char *);

int pthread_create(pthread_t *restrict res, const pthread_attr_t *restrict attr, void *(*entry)(void *), void *restrict arg)
{
	int ret;
	size_t size = DEFAULT_STACK_SIZE + DEFAULT_GUARD_SIZE;
	size_t guard = DEFAULT_GUARD_SIZE;
	struct pthread *self = pthread_self(), *new;
	unsigned char *map, *stack, *tsd;
	unsigned flags = 0x7d8f00;
	int do_sched = 0;

	if (!self) return ENOSYS;
	if (!libc.threaded) {
		for (FILE *f=libc.ofl_head; f; f=f->next)
			init_file_lock(f);
		init_file_lock(__stdin_used);
		init_file_lock(__stdout_used);
		init_file_lock(__stderr_used);
		libc.threaded = 1;
	}

	__acquire_ptc();

	if (attr && attr->_a_stackaddr) {
		map = 0;
		tsd = (void *)(attr->_a_stackaddr-__pthread_tsd_size & -16);
	} else {
		if (attr) {
			guard = ROUND(attr->_a_guardsize + DEFAULT_GUARD_SIZE);
			size = guard + ROUND(attr->_a_stacksize
				+ DEFAULT_STACK_SIZE + libc.tls_size);
		}
		size += __pthread_tsd_size;
		if (guard) {
			map = mmap(0, size, PROT_NONE, MAP_PRIVATE|MAP_ANON, -1, 0);
			if (map == MAP_FAILED) return EAGAIN;
			if (mprotect(map+guard, size-guard, PROT_READ|PROT_WRITE)) {
				munmap(map, size);
				return EAGAIN;
			}
		} else {
			map = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
			if (map == MAP_FAILED) return EAGAIN;
		}
		tsd = map + size - __pthread_tsd_size;
	}
	new = __copy_tls(stack = tsd - libc.tls_size);
	new->map_base = map;
	new->map_size = size;
	new->pid = self->pid;
	new->errno_ptr = &new->errno_val;
	new->start = entry;
	new->start_arg = arg;
	new->self = new;
	new->tsd = (void *)tsd;
	if (attr && attr->_a_detach) {
		new->detached = 1;
		flags -= 0x200000;
	}
	if (attr && attr->_a_sched) {
		do_sched = new->startlock[0] = 1;
		__syscall(SYS_rt_sigprocmask, SIG_BLOCK,
			SIGALL_SET, self->sigmask, __SYSCALL_SSLEN);
	}
	new->unblock_cancel = self->cancel;
	new->canary = self->canary;

	a_inc(&libc.threads_minus_1);
	ret = __clone(start, stack, flags, new, &new->tid, TP_ADJ(new), &new->tid);

	__release_ptc();

	if (do_sched) {
		__syscall(SYS_rt_sigprocmask, SIG_SETMASK,
			new->sigmask, 0, __SYSCALL_SSLEN);
	}

	if (ret < 0) {
		a_dec(&libc.threads_minus_1);
		munmap(map, size);
		return EAGAIN;
	}

	if (do_sched) {
		ret = __syscall(SYS_sched_setscheduler, new->tid,
			attr->_a_policy, &attr->_a_prio);
		a_store(new->startlock, ret<0 ? 2 : 0);
		__wake(new->startlock, 1, 1);
		if (ret < 0) return -ret;
	}

	*res = new;
	return 0;
}
