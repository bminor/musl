#include <unistd.h>
#include "syscall.h"

int fchdir(int fd)
{
	return syscall1(__NR_fchdir, fd);
}
