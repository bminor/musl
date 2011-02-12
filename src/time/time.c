#define SYSCALL_RETURN_ERRNO
#include <time.h>
#include <sys/time.h>
#include "syscall.h"

time_t time(time_t *t)
{
	struct timeval tv;
	syscall2(__NR_gettimeofday, (long)&tv, 0);
	if (t) *t = tv.tv_sec;
	return tv.tv_sec;
}
