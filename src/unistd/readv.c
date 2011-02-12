#include <sys/uio.h>
#include "syscall.h"
#include "libc.h"

ssize_t readv(int fd, const struct iovec *iov, int count)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall3(__NR_readv, fd, (long)iov, count);
	CANCELPT_END;
	return r;
}
