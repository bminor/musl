#include <sys/stat.h>
#include "syscall.h"
#include "libc.h"

int lstat(const char *path, struct stat *buf)
{
	return syscall(SYS_lstat, path, buf);
}

LFS64(lstat);
