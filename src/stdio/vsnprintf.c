#include "stdio_impl.h"

static size_t sn_write(FILE *f, const unsigned char *s, size_t l)
{
	/* pretend to succeed, but discard data */
	return l;
}

int vsnprintf(char *s, size_t n, const char *fmt, va_list ap)
{
	int r;
	FILE f;
	unsigned char buf[1];

	memset(&f, 0, sizeof(FILE));
	f.lbf = EOF;
	f.write = sn_write;
	f.buf_size = 1;
	f.buf = buf;
	if (n > INT_MAX) {
		errno = EOVERFLOW;
		return -1;
	} else if (n > 0) {
		if (n > (char *)0+SIZE_MAX-s) n = (char *)0+SIZE_MAX-s;
		f.wpos = s;
		f.wbase = f.wend = s+n-1;
		f.wstop = f.wend - 1;
	}
	r = vfprintf(&f, fmt, ap);
	/* wpos points just after last byte written, or to s+n-1 (wbase) */
	*f.wpos = 0;
	return r;
}
