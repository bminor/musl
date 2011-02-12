#include <unistd.h>
#include "syscall.h"

int rmdir(const char *path)
{
	return syscall1(__NR_rmdir, (long)path);
}
