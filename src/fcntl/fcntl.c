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
	if (cmd == F_DUPFD_CLOEXEC) {
		int ret = __syscall(SYS_fcntl, fd, F_DUPFD_CLOEXEC, arg);
		if (ret != -EINVAL) {
			if (ret >= 0)
				__syscall(SYS_fcntl, ret, F_SETFD, FD_CLOEXEC);
			return __syscall_ret(ret);
		}
		ret = __syscall(SYS_fcntl, fd, F_DUPFD_CLOEXEC, 0);
		if (ret != -EINVAL) {
			if (ret >= 0) __syscall(SYS_close, ret);
			return __syscall_ret(-EINVAL);
		}
		ret = __syscall(SYS_fcntl, fd, F_DUPFD, arg);
		if (ret >= 0) __syscall(SYS_fcntl, ret, F_SETFD, FD_CLOEXEC);
		return __syscall_ret(ret);
	}
	return syscall(SYS_fcntl, fd, cmd, arg);
}
