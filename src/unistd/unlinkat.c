#include <unistd.h>
#include "syscall.h"

int unlinkat(int fd, const char *path, int flag)
{
	return syscall3(__NR_unlinkat, fd, (long)path, flag);
}
