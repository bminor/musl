#include <sys/statvfs.h>
#include "syscall.h"
#include "libc.h"

int statvfs(const char *path, struct statvfs *buf)
{
#ifdef SYS_statfs64
	return syscall(SYS_statfs64, path, sizeof *buf, buf);
#else
	return syscall(SYS_statfs, path, buf);
#endif
}

weak_alias(statvfs, statfs);

LFS64(statvfs);
LFS64(statfs);
