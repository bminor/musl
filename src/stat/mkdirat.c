#include <sys/stat.h>
#include "syscall.h"

int mkdirat(int fd, const char *path, mode_t mode)
{
	return syscall3(__NR_mkdirat, fd, (long)path, mode);
}
