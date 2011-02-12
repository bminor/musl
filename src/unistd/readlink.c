#include <unistd.h>
#include "syscall.h"

int readlink(const char *path, char *buf, size_t bufsize)
{
	return syscall3(__NR_readlink, (long)path, (long)buf, bufsize);
}
