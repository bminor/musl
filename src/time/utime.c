#include <utime.h>
#include "syscall.h"

int utime(const char *path, const struct utimbuf *times)
{
	long ktimes[2];
	if (times) {
		ktimes[0] = times->actime;
		ktimes[1] = times->modtime;
	}
	return syscall2(__NR_utime, (long)path, times ? (long)ktimes : 0);
}
