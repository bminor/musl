#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int close(int fd)
{
	return syscall_cp(SYS_close, fd);
}
