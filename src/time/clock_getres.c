#include <time.h>
#include "syscall.h"

int clock_getres(clockid_t clk, struct timespec *ts)
{
	return syscall2(__NR_clock_getres, clk, (long)ts);
}
