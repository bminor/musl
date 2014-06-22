#include <sys/stat.h>
#include "libc.h"

int __fxstat(int ver, int fd, struct stat *buf)
{
	return fstat(fd, buf);
}

int __fxstatat(int ver, int fd, const char *path, struct stat *buf, int flag)
{
	return fstatat(fd, path, buf, flag);
}

int __lxstat(int ver, const char *path, struct stat *buf)
{
	return lstat(path, buf);
}

int __xstat(int ver, const char *path, struct stat *buf)
{
	return stat(path, buf);
}

LFS64(__fxstat);
LFS64(__fxstatat);
LFS64(__lxstat);
LFS64(__xstat);
