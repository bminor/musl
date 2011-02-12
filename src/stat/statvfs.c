#include <sys/statvfs.h>
#include "syscall.h"
#include "libc.h"

int statvfs(const char *path, struct statvfs *buf)
{
	return syscall2(__NR_statfs64, (long)path, (long)buf);
}

weak_alias(statvfs, statfs);

LFS64(statvfs);
LFS64(statfs);
