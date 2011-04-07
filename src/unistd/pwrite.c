#include <unistd.h>
#include "syscall.h"
#include "libc.h"

ssize_t pwrite(int fd, const void *buf, size_t size, off_t ofs)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall(SYS_pwrite, fd, buf, size, __SYSCALL_LL(ofs));
	CANCELPT_END;
	return r;
}

LFS64(pwrite);
