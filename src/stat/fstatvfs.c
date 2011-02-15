#include <sys/statvfs.h>
#include "syscall.h"
#include "libc.h"

int fstatvfs(int fd, struct statvfs *buf)
{
	return syscall2(__NR_fstatfs, fd, (long)buf);
}

weak_alias(fstatvfs, fstatfs);

LFS64(fstatvfs);
LFS64(fstatfs);
