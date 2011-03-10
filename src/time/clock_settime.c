#include <time.h>
#include "syscall.h"

int clock_settime(clockid_t clk, const struct timespec *ts)
{
	return syscall2(__NR_clock_settime, clk, (long)ts);
}
