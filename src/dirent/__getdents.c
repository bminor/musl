#include <dirent.h>
#include "syscall.h"
#include "libc.h"

int __getdents(int fd, struct dirent *buf, size_t len)
{
	return syscall3(__NR_getdents64, fd, (long)buf, len);
}

weak_alias(__getdents, getdents);

LFS64(getdents);
