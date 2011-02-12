#include <sys/stat.h>
#include "syscall.h"
#include "libc.h"

int lstat(const char *path, struct stat *buf)
{
	return syscall2(__NR_lstat64, (long)path, (long)buf);
}

LFS64(lstat);
