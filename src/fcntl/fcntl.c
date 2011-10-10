#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include "syscall.h"
#include "libc.h"

int fcntl(int fd, int cmd, ...)
{
	long arg;
	va_list ap;
	va_start(ap, cmd);
	arg = va_arg(ap, long);
	va_end(ap);
	if (cmd == F_SETFL) arg |= O_LARGEFILE;
	if (cmd == F_SETLKW) return syscall_cp(SYS_fcntl, fd, cmd, arg);
	if (cmd == F_GETOWN) return __syscall(SYS_fcntl, fd, cmd, arg);
	return syscall(SYS_fcntl, fd, cmd, arg);
}

LFS64(fcntl);
