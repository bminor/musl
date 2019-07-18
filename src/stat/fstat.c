#define _BSD_SOURCE
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"

int fstat(int fd, struct stat *st)
{
	return fstatat(fd, "", st, AT_EMPTY_PATH);
}

weak_alias(fstat, fstat64);
