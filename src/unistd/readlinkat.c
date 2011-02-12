#include <unistd.h>
#include "syscall.h"

int readlinkat(int fd, const char *path, char *buf, size_t bufsize)
{
	return syscall4(__NR_readlinkat, fd, (long)path, (long)buf, bufsize);
}
