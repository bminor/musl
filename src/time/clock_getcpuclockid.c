#include <time.h>
#include <errno.h>
#include <unistd.h>
#include "syscall.h"

int clock_getcpuclockid(pid_t pid, clockid_t *clk)
{
	if (pid && pid != getpid()) return EPERM;
	*clk = CLOCK_PROCESS_CPUTIME_ID;
	return 0;
}
