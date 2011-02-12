#include <sys/stat.h>
#include "syscall.h"

int fchmodat(int fd, const char *path, mode_t mode, int flag)
{
	return syscall4(__NR_fchmodat, fd, (long)path, mode, flag);
}
