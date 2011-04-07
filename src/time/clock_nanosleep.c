#include <time.h>
#include "syscall.h"
#include "libc.h"

int clock_nanosleep(clockid_t clk, int flags, const struct timespec *req, struct timespec *rem)
{
	int ret;
	CANCELPT_BEGIN;
	ret = -__syscall(SYS_clock_nanosleep, clk, flags, req, rem);
	CANCELPT_END;
	return ret;
}
