#include <pthread.h>
#include <time.h>
#include <errno.h>
#include "futex.h"
#include "syscall.h"

static int do_wait(volatile int *addr, int val, clockid_t clk, const struct timespec *at, int cp, int priv)
{
	int r, flag = 0;
	struct timespec to, *top=0;

	if (!at) goto notimeout;
	if (at->tv_nsec >= 1000000000UL)
		return EINVAL;
	if (clk == CLOCK_REALTIME || clk == CLOCK_MONOTONIC) {
		if (clk == CLOCK_REALTIME) flag = FUTEX_CLOCK_REALTIME;
		if (cp) r = -__syscall_cp(SYS_futex, addr, FUTEX_WAIT_BITSET|flag, val, at, 0, -1);
		else r = -__syscall(SYS_futex, addr, FUTEX_WAIT_BITSET|flag, val, at, 0, -1);
		if (r != EINVAL && r != ENOSYS) goto done;
	}
	if (clock_gettime(clk, &to)) return EINVAL;
	to.tv_sec = at->tv_sec - to.tv_sec;
	if ((to.tv_nsec = at->tv_nsec - to.tv_nsec) < 0) {
		to.tv_sec--;
		to.tv_nsec += 1000000000;
	}
	if (to.tv_sec < 0) return ETIMEDOUT;
	top = &to;
notimeout:
	if (cp) r = -__syscall_cp(SYS_futex, addr, FUTEX_WAIT, val, top);
	else r = -__syscall(SYS_futex, addr, FUTEX_WAIT, val, top);
done:
	if (r == EINTR || r == EINVAL || r == ETIMEDOUT) return r;
	return 0;
}

int __timedwait(volatile int *addr, int val, clockid_t clk, const struct timespec *at, void (*cleanup)(void *), void *arg, int priv)
{
	int r;
	if (cleanup) {
		pthread_cleanup_push(cleanup, arg);
		r = do_wait(addr, val, clk, at, 1, priv);
		pthread_cleanup_pop(0);
	} else {
		r = do_wait(addr, val, clk, at, 0, priv);
	}
	return r;
}
