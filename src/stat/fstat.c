#include <sys/stat.h>
#include "syscall.h"
#include "libc.h"

int fstat(int fd, struct stat *buf)
{
	return syscall2(__NR_fstat64, fd, (long)buf);
}

LFS64(fstat);
