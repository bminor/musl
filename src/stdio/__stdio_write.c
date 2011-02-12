#include "stdio_impl.h"

size_t __stdio_write(FILE *f, const unsigned char *buf, size_t len)
{
	const unsigned char *stop = buf+len;
	ssize_t cnt = 1;
	for (; buf<stop && (cnt=__syscall_write(f->fd, buf, len))>0; buf+=cnt);
	return len-(stop-buf);
}
