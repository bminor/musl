#include <unistd.h>
#include "syscall.h"

int fchdir(int fd)
{
	return syscall(SYS_fchdir, fd);
}
