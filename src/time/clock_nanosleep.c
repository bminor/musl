#define SYSCALL_RETURN_ERRNO
#include <time.h>
#include "syscall.h"

int clock_nanosleep(clockid_t clk, int flags, const struct timespec *req, struct timespec *rem)
{
	return syscall4(__NR_clock_nanosleep, clk, flags, (long)req, (long)rem);
}
