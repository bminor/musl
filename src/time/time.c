#include <time.h>
#include <sys/time.h>
#include "syscall.h"

time_t time(time_t *t)
{
	struct timeval tv;
	__syscall(SYS_gettimeofday, &tv, 0);
	if (t) *t = tv.tv_sec;
	return tv.tv_sec;
}
