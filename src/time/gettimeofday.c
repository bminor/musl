#define SYSCALL_RETURN_ERRNO
#include <sys/time.h>
#include "syscall.h"

int gettimeofday(struct timeval *tv, void *tz)
{
	syscall2(__NR_gettimeofday, (long)tv, 0);
	return 0;
}
