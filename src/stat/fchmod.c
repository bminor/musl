#include <sys/stat.h>
#include "syscall.h"

int fchmod(int fd, mode_t mode)
{
	return syscall(SYS_fchmod, fd, mode);
}
