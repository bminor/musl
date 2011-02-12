#include <sys/stat.h>
#include "syscall.h"
#include "libc.h"

int stat(const char *path, struct stat *buf)
{
	return syscall2(__NR_stat64, (long)path, (long)buf);
}

LFS64(stat);
