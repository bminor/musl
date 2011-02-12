#include <sys/stat.h>
#include "syscall.h"

int mknodat(int fd, const char *path, mode_t mode, dev_t dev)
{
	return syscall4(__NR_mknodat, fd, (long)path, mode, dev & 0xffff);
}
