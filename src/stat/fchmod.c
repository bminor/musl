#include <sys/stat.h>
#include "syscall.h"

int fchmod(int fd, mode_t mode)
{
	return syscall2(__NR_fchmod, fd, mode);
}
