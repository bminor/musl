#include <time.h>
#include <errno.h>
#include "syscall.h"

int __clock_nanosleep(clockid_t clk, int flags, const struct timespec *req, struct timespec *rem)
{
	if (clk == CLOCK_THREAD_CPUTIME_ID) return EINVAL;
	if (clk == CLOCK_REALTIME && !flags)
		return -__syscall_cp(SYS_nanosleep, req, rem);
	return -__syscall_cp(SYS_clock_nanosleep, clk, flags, req, rem);
}

weak_alias(__clock_nanosleep, clock_nanosleep);
