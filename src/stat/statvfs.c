#include <sys/statvfs.h>
#include "syscall.h"
#include "libc.h"

int statvfs(const char *path, struct statvfs *buf)
{
	return syscall(SYS_statfs, path, sizeof *buf, buf);
}

weak_alias(statvfs, statfs);

LFS64(statvfs);
LFS64(statfs);
