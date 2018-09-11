#include <dirent.h>
#include "syscall.h"

int getdents(int fd, struct dirent *buf, size_t len)
{
	return syscall(SYS_getdents, fd, buf, len);
}

LFS64(getdents);
