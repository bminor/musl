#include <unistd.h>
#include "syscall.h"
#include "libc.h"

ssize_t read(int fd, void *buf, size_t count)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = __syscall_read(fd, buf, count);
	CANCELPT_END;
	return r;
}
