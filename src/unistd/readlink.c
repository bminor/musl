#include <unistd.h>
#include "syscall.h"

ssize_t readlink(const char *path, char *buf, size_t bufsize)
{
	return syscall(SYS_readlink, path, buf, bufsize);
}
