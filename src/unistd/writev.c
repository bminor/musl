#include <sys/uio.h>
#include "syscall.h"
#include "libc.h"

ssize_t writev(int fd, const struct iovec *iov, int count)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall(SYS_writev, fd, iov, count);
	CANCELPT_END;
	return r;
}
