#include <time.h>
#include <errno.h>
#include "futex.h"
#define SYSCALL_RETURN_ERRNO
#include "syscall.h"
#include <stdio.h>
int __timedwait(volatile int *addr, int val, clockid_t clk, const struct timespec *at, int priv)
{
	struct timespec to;
	if (at) {
		clock_gettime(clk, &to);
		to.tv_sec = at->tv_sec - to.tv_sec;
		if ((to.tv_nsec = at->tv_nsec - to.tv_nsec) < 0) {
			to.tv_sec--;
			to.tv_nsec += 1000000000;
		}
		if (to.tv_sec < 0) return ETIMEDOUT;
	}
	if (priv) priv = 128; priv=0;
	return syscall4(__NR_futex, (long)addr, FUTEX_WAIT | priv, val, at ? (long)&to : 0);
}
