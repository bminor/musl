#include <time.h>
#include "syscall.h"

int clock_gettime(clockid_t clk, struct timespec *ts)
{
	return syscall2(__NR_clock_gettime, clk, (long)ts);
}
