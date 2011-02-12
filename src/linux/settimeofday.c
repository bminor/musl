#include <sys/time.h>
#include "syscall.h"

int settimeofday(const struct timeval *tv, void *tz)
{
	return syscall2(__NR_settimeofday, (long)tv, 0);
}
