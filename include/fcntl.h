#ifndef	_FCNTL_H
#define	_FCNTL_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_off_t
#define __NEED_pid_t
#define __NEED_mode_t

#include <bits/alltypes.h>

#include <bits/fcntl.h>

struct flock
{
	short l_type;
	short l_whence;
	off_t l_start;
	off_t l_len;
	pid_t l_pid;
};

int creat(const char *, mode_t);
int fcntl(int, int, ...);
int open(const char *, int, ...);
int openat(int, const char *, int, ...);
int posix_fadvise(int, off_t, off_t, int);
int posix_fallocate(int, off_t, off_t);

#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifdef __cplusplus
}
#endif

#endif
