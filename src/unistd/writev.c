#include <sys/uio.h>
#include "syscall.h"
#include "libc.h"

ssize_t writev(int fd, const struct iovec *iov, int count)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall3(__NR_writev, fd, (long)iov, count);
	CANCELPT_END;
	return r;
}
