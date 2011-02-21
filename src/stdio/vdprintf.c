#include "stdio_impl.h"

static size_t wrap_write(FILE *f, const unsigned char *buf, size_t len)
{
	return __stdio_write(f, buf, len);
}

int vdprintf(int fd, const char *fmt, va_list ap)
{
	int r;
	char buf[BUFSIZ];
	FILE f = {
		.fd = fd, .lbf = EOF, .write = wrap_write,
		.buf = buf+UNGET, .buf_size = sizeof buf - UNGET
	};
	r = vfprintf(&f, fmt, ap);
	__oflow(&f);
	return r;
}
