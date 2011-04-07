#include "pthread_impl.h"

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
	int init;
} rs;

static void rsyscall_handler(int sig, siginfo_t *si, void *ctx)
{
	struct pthread *self = __pthread_self();
	long r;

	if (!rs.hold || rs.cnt == libc.threads_minus_1) return;

	/* Threads which have already decremented themselves from the
	 * thread count must not increment rs.cnt or otherwise act. */
	if (self->dead) {
		sigfillset(&((ucontext_t *)ctx)->uc_sigmask);
		return;
	}

	r = __syscall(rs.nr, rs.arg[0], rs.arg[1],
		rs.arg[2], rs.arg[3], rs.arg[4], rs.arg[5]);
	if (r < 0) rs.err=-r;

	a_inc(&rs.cnt);
	__wake(&rs.cnt, 1, 1);
	while(rs.hold)
		__wait(&rs.hold, 0, 1, 1);
	a_dec(&rs.cnt);
	if (!rs.cnt) __wake(&rs.cnt, 1, 1);
}

int __rsyscall(int nr, long a, long b, long c, long d, long e, long f)
{
	int i, ret;
	sigset_t set = { 0 };
	struct pthread *self;

	if (!libc.threads_minus_1)
		return syscall(nr, a, b, c, d, e, f);

	self = __pthread_self();

	LOCK(&rs.lock);
	while ((i=rs.blocks))
		__wait(&rs.blocks, 0, i, 1);

	sigfillset(&set);
	__libc_sigprocmask(SIG_BLOCK, &set, &set);

	if (!rs.init) {
		struct sigaction sa = {
			.sa_flags = SA_SIGINFO | SA_RESTART,
			.sa_sigaction = rsyscall_handler,
			.sa_mask = set
		};
		sigfillset(&sa.sa_mask);
		sa.sa_sigaction = rsyscall_handler;
		__libc_sigaction(SIGSYSCALL, &sa, 0);
		rs.init = 1;
	}

	rs.nr = nr;
	rs.arg[0] = a; rs.arg[1] = b;
	rs.arg[2] = c; rs.arg[3] = d;
	rs.arg[4] = d; rs.arg[5] = f;
	rs.err = 0;
	rs.cnt = 0;
	rs.hold = 1;

	/* Dispatch signals until all threads respond */
	for (i=libc.threads_minus_1; i; i--)
		sigqueue(self->pid, SIGSYSCALL, (union sigval){0});
	while ((i=rs.cnt) < libc.threads_minus_1) {
		sigqueue(self->pid, SIGSYSCALL, (union sigval){0});
		__wait(&rs.cnt, 0, i, 1);
	}

	/* Handle any lingering signals with no-op */
	__libc_sigprocmask(SIG_UNBLOCK, &set, &set);

	/* Resume other threads' signal handlers and wait for them */
	rs.hold = 0;
	__wake(&rs.hold, -1, 0);
	while((i=rs.cnt)) __wait(&rs.cnt, 0, i, 1);

	if (rs.err) errno = rs.err, ret = -1;
	else ret = syscall(nr, a, b, c, d, e, f);

	UNLOCK(&rs.lock);
	return ret;
}

void __rsyscall_lock()
{
	a_inc(&rs.blocks);
	while (rs.lock) __wait(&rs.lock, 0, 1, 1);
}

void __rsyscall_unlock()
{
	a_dec(&rs.blocks);
	if (rs.lock) __wake(&rs.blocks, 1, 1);
}
