#define _BSD_SOURCE
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "syscall.h"
#include "kstat.h"

int fstatat(int fd, const char *restrict path, struct stat *restrict st, int flag)
{
	int ret;
	struct kstat kst;

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
		ret = __syscall(SYS_lstat, path, &kst);
#endif
#ifdef SYS_stat
	else if ((fd == AT_FDCWD || *path=='/') && !flag)
		ret = __syscall(SYS_stat, path, &kst);
#endif
	else ret = __syscall(SYS_fstatat, fd, path, &kst, flag);

	if (ret) return __syscall_ret(ret);

	*st = (struct stat){
		.st_dev = kst.st_dev,
		.st_ino = kst.st_ino,
		.st_mode = kst.st_mode,
		.st_nlink = kst.st_nlink,
		.st_uid = kst.st_uid,
		.st_gid = kst.st_gid,
		.st_rdev = kst.st_rdev,
		.st_size = kst.st_size,
		.st_blksize = kst.st_blksize,
		.st_blocks = kst.st_blocks,
		.st_atim.tv_sec = kst.st_atime_sec,
		.st_atim.tv_nsec = kst.st_atime_nsec,
		.st_mtim.tv_sec = kst.st_mtime_sec,
		.st_mtim.tv_nsec = kst.st_mtime_nsec,
		.st_ctim.tv_sec = kst.st_ctime_sec,
		.st_ctim.tv_nsec = kst.st_ctime_nsec,
	};

	return 0;
}

weak_alias(fstatat, fstatat64);
