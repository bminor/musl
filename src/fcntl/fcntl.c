#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
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
	if (cmd == F_GETOWN) {
		struct f_owner_ex ex;
		int ret = __syscall(SYS_fcntl, fd, F_GETOWN_EX, &ex);
		if (ret == -EINVAL) return __syscall(SYS_fcntl, fd, cmd, arg);
		if (ret) return __syscall_ret(ret);
		return ex.type == F_OWNER_PGRP ? -ex.pid : ex.pid;
	}
	return syscall(SYS_fcntl, fd, cmd, arg);
}
