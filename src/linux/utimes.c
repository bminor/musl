#include <sys/time.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int utimes(const char *path, const struct timeval times[2])
{
	long ktimes[2];
	if (times) {
		ktimes[0] = times[0].tv_sec;
		ktimes[1] = times[1].tv_sec;
	}
	return syscall2(__NR_utime, (long)path, times ? (long)ktimes : 0);
}
