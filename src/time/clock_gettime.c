#include <time.h>
#include "syscall.h"

int clock_gettime(clockid_t clk, struct timespec *ts)
{
	return syscall(SYS_clock_gettime, clk, ts);
}
