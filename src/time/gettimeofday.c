#include <sys/time.h>
#include "syscall.h"

int gettimeofday(struct timeval *tv, void *tz)
{
	__syscall(SYS_gettimeofday, tv, 0);
	return 0;
}
