#include <sys/statvfs.h>
#include "syscall.h"
#include "libc.h"

int fstatvfs(int fd, struct statvfs *buf)
{
	return syscall(SYS_fstatfs, fd, sizeof *buf, buf);
}

weak_alias(fstatvfs, fstatfs);

LFS64(fstatvfs);
LFS64(fstatfs);
