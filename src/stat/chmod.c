#include <sys/stat.h>
#include "syscall.h"

int chmod(const char *path, mode_t mode)
{
	return syscall2(__NR_chmod, (long)path, mode);
}
