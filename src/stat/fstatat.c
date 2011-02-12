#include <sys/stat.h>
#include "syscall.h"
#include "libc.h"

int fstatat(int fd, const char *path, struct stat *buf, int flag)
{
	return syscall4(__NR_fstatat64, fd, (long)path, (long)buf, flag);
}

LFS64(fstatat);
