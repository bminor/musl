#include <sys/statvfs.h>
#include "syscall.h"
#include "libc.h"

int fstatvfs(int fd, struct statvfs *buf)
{
#ifdef SYS_fstatfs64
	return syscall(SYS_fstatfs64, fd, sizeof *buf, buf);
#else
	return syscall(SYS_fstatfs, fd, buf);
#endif
}

weak_alias(fstatvfs, fstatfs);

LFS64(fstatvfs);
LFS64(fstatfs);
