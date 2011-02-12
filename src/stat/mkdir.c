#include <sys/stat.h>
#include "syscall.h"

int mkdir(const char *path, mode_t mode)
{
	return syscall2(__NR_mkdir, (long)path, mode);
}
