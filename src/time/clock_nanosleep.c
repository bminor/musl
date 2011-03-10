#define SYSCALL_RETURN_ERRNO
#include <time.h>
#include "syscall.h"
#include "libc.h"

int clock_nanosleep(clockid_t clk, int flags, const struct timespec *req, struct timespec *rem)
{
	int ret;
	CANCELPT_BEGIN;
	ret = syscall4(__NR_clock_nanosleep, clk, flags, (long)req, (long)rem);
	CANCELPT_END;
	return ret;
}
