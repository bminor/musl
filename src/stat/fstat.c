#include <sys/stat.h>
#include <errno.h>
#include "syscall.h"
#include "libc.h"

void __procfdname(char *, unsigned);

int fstat(int fd, struct stat *st)
{
	int ret = __syscall(SYS_fstat, fd, st);
	if (ret != -EBADF || fd < 0) return __syscall_ret(ret);

	char buf[15+3*sizeof(int)];
	__procfdname(buf, fd);
	return syscall(SYS_stat, buf, st);
}

LFS64(fstat);
