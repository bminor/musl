#include "pthread_impl.h"

#define pthread_self __pthread_self

static void docancel(struct pthread *self)
{
	struct __ptcb cb = { .__next = self->cancelbuf };
	__pthread_unwind_next(&cb);
}

static void cancel_handler(int sig, siginfo_t *si, void *ctx)
{
	struct pthread *self = pthread_self();
	self->cancel = 1;
	if (self->canceldisable || (!self->cancelasync && !self->cancelpoint))
		return;
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
	if (rs.cnt == libc.threads_minus_1) return;

	if (syscall6(rs.nr, rs.arg[0], rs.arg[1], rs.arg[2],
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
	struct pthread *self = pthread_self();
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
	else ret = syscall6(nr, a, b, c, d, e, f);

	UNLOCK(&rs.lock);
	return ret;
}

static void cancelpt(int x)
{
	struct pthread *self = pthread_self();
	if (self->canceldisable) return;
	self->cancelpoint = x;
	if (self->cancel) docancel(self);
}

static void init_threads()
{
	struct sigaction sa = { .sa_flags = SA_SIGINFO | SA_RESTART };
	libc.lock = __lock;
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
	pthread_exit(self->start(self->start_arg));
	return 0;
}

#undef pthread_self

#define CLONE_MAGIC 0x7d0f00
int __clone(int (*)(void *), void *, int, void *, pid_t *, void *, pid_t *);

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
	guard = ROUND(attr->__guardsize + DEFAULT_GUARD_SIZE);
	size = guard + ROUND(attr->__stacksize + DEFAULT_STACK_SIZE);
	size += __pthread_tsd_size;
	map = mmap(0, size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANON, -1, 0);
	if (!map) return EAGAIN;
	mprotect(map, guard, PROT_NONE);

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
	new->detached = attr->__detach;
	new->attr = *attr;
	memcpy(new->tlsdesc, self->tlsdesc, sizeof new->tlsdesc);
	new->tlsdesc[1] = (uintptr_t)new;
	stack = (void *)((uintptr_t)new-1 & ~(uintptr_t)15);

	/* We must synchronize new thread creation with rsyscall
	 * delivery. This looks to be the least expensive way: */
	a_inc(&rs.blocks);
	while (rs.lock) __wait(&rs.lock, 0, 1, 1);

	a_inc(&libc.threads_minus_1);
	ret = __clone(start, stack, CLONE_MAGIC, new,
		&new->tid, &new->tlsdesc, &new->tid);

	a_dec(&rs.blocks);
	if (rs.lock) __wake(&rs.blocks, 1, 1);

	if (ret < 0) {
		a_dec(&libc.threads_minus_1);
		munmap(map, size);
		return -ret;
	}
	*res = new;
	return 0;
}
