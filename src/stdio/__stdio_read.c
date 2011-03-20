#include "stdio_impl.h"

size_t __stdio_read(FILE *f, unsigned char *buf, size_t len)
{
	return syscall(SYS_read, f->fd, buf, len);
}
