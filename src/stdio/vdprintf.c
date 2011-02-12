#include "stdio_impl.h"

int vdprintf(int fd, const char *fmt, va_list ap)
{
	int r;
	char buf[BUFSIZ];
	FILE f = {
		.fd = fd, .lbf = EOF, .write = __stdio_write,
		.buf = buf+UNGET, .buf_size = sizeof buf - UNGET
	};
	r = vfprintf(&f, fmt, ap);
	__oflow(&f);
	return r;
}
