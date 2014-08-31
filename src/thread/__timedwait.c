#include <pthread.h>
#include <time.h>
#include <errno.h>
#include "futex.h"
#include "syscall.h"

int __pthread_setcancelstate(int, int *);
int __clock_gettime(clockid_t, struct timespec *);

int __timedwait(volatile int *addr, int val,
	clockid_t clk, const struct timespec *at,
	void (*cleanup)(void *), void *arg, int priv)
{
	int r, cs;
	struct timespec to, *top=0;

	if (priv) priv = 128;

	if (at) {
		if (at->tv_nsec >= 1000000000UL) return EINVAL;
		if (__clock_gettime(clk, &to)) return EINVAL;
		to.tv_sec = at->tv_sec - to.tv_sec;
		if ((to.tv_nsec = at->tv_nsec - to.tv_nsec) < 0) {
			to.tv_sec--;
			to.tv_nsec += 1000000000;
		}
		if (to.tv_sec < 0) return ETIMEDOUT;
		top = &to;
	}

	if (!cleanup) __pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	pthread_cleanup_push(cleanup, arg);

	r = -__syscall_cp(SYS_futex, addr, FUTEX_WAIT|priv, val, top);
	if (r == ENOSYS) r = -__syscall_cp(SYS_futex, addr, FUTEX_WAIT, val, top);
	if (r != EINTR && r != ETIMEDOUT) r = 0;

	pthread_cleanup_pop(0);
	if (!cleanup) __pthread_setcancelstate(cs, 0);

	return r;
}
