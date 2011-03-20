#include <utime.h>
#include "syscall.h"

int utime(const char *path, const struct utimbuf *times)
{
	return syscall(SYS_utime, path, times);
}
