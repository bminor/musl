#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include "syscall.h"
#include "libc.h"

int fcntl(int fd, int cmd, ...)
{
	int r;
	long arg;
	va_list ap;
	va_start(ap, cmd);
	arg = va_arg(ap, long);
	va_end(ap);
	if (cmd == F_SETFL) arg |= O_LARGEFILE;
	if (cmd == F_SETLKW) CANCELPT_BEGIN;
	r = __syscall_fcntl(fd, cmd, arg);
	if (cmd == F_SETLKW) CANCELPT_END;
	return r;
}

LFS64(fcntl);
