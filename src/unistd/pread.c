#include <unistd.h>
#include "syscall.h"
#include "libc.h"

ssize_t pread(int fd, void *buf, size_t size, off_t ofs)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall5(__NR_pread64, fd, (long)buf, size, SYSCALL_LL(ofs));
	CANCELPT_END;
	return r;
}

LFS64(pread);
