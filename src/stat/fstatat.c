#define _BSD_SOURCE
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "syscall.h"

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	int ret;

	if (flag==AT_EMPTY_PATH && fd>=0 && !*path) {
		ret = __syscall(SYS_fstat, fd, st);
		if (ret==-EBADF && __syscall(SYS_fcntl, fd, F_GETFD)>=0) {
			ret = __syscall(SYS_fstatat, fd, path, st, flag);
			if (ret==-EINVAL) {
				char buf[15+3*sizeof(int)];
				__procfdname(buf, fd);
#ifdef SYS_stat
				ret = __syscall(SYS_stat, buf, st);
#else
				ret = __syscall(SYS_fstatat, AT_FDCWD, buf, st, 0);
#endif
			}
		}
	}
#ifdef SYS_lstat
	else if ((fd == AT_FDCWD || *path=='/') && flag==AT_SYMLINK_NOFOLLOW)
		ret = __syscall(SYS_lstat, path, st);
#endif
#ifdef SYS_stat
	else if ((fd == AT_FDCWD || *path=='/') && !flag)
		ret = __syscall(SYS_stat, path, st);
#endif
	else ret = __syscall(SYS_fstatat, fd, path, st, flag);

	return __syscall_ret(ret);
}

weak_alias(fstatat, fstatat64);
