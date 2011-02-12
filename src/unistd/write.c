#include <unistd.h>
#include "syscall.h"
#include "libc.h"

ssize_t write(int fd, const void *buf, size_t count)
{
	int r;
	CANCELPT_BEGIN;
	r = __syscall_write(fd, buf, count);
	CANCELPT_END;
	return r;
}
