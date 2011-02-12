#include "stdio_impl.h"

FILE *__fopen_rb_ca(const char *filename, FILE *f, unsigned char *buf, size_t len)
{
	memset(f, 0, sizeof *f);

	f->fd = __syscall_open(filename, O_RDONLY, 0);
	if (f->fd < 0) return 0;

	f->flags = F_NOWR | F_PERM;
	f->buf = buf + UNGET;
	f->buf_size = len - UNGET;
	f->read = __stdio_read;
	f->seek = __stdio_seek;
	f->close = __stdio_close;

	return f;
}
