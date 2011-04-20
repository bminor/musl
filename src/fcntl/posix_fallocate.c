#include <fcntl.h>
#include "syscall.h"

int posix_fallocate(int fd, off_t base, off_t len)
{
	return -__syscall(SYS_fallocate, fd, __SYSCALL_LL(base),
		__SYSCALL_LL(len));
}
