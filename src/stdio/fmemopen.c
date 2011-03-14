#if 0
#include "stdio_impl.h"

static ssize_t mread(FILE *f, unsigned char *buf, size_t len)
{
	size_t rem = f->memsize - f->mempos;
	if (len > rem) len = rem;
	memcpy(buf, f->membuf+f->mempos, len);
	f->mempos += len;
	return len;
}

static ssize_t mwrite(FILE *f, const unsigned char *buf, size_t len)
{
	size_t rem;
	if (f->memmode == 'a') f->mempos = f->memsize;
	rem = f->memlim - f->mempos;
	if (len > rem) len = rem;
	memcpy(f->membuf+f->mempos, buf, len);
	f->mempos += len;
	if (f->mempos >= f->memsize) {
		f->memsize = f->mempos;
	}
	return len;
}

FILE *fmemopen(void *buf, size_t size, const char *mode)
{
	FILE *f;
	int plus = !!strchr(mode, '+');
	
	if (!size || !strchr("rwa", *mode)) {
		errno = EINVAL;
		return 0;
	}

	if (!buf && size > SIZE_MAX-sizeof(FILE)-BUFSIZ-UNGET) {
		errno = ENOMEM;
		return 0;
	}

	f = calloc(sizeof(FILE) + UNGET + BUFSIZ + (buf?0:size), 1);
	if (!f) return 0;
	f->fd = -1;
	f->lbf = EOF;
	f->buf = (unsigned char *)(f+1) + UNGET;
	f->buf_size = BUFSIZ;
	if (!buf) buf = f->buf + BUFSIZ;
	
	if (!plus) f->flags = (*mode == 'r') ? F_NOWR : F_NORD;
	if (*mode == 'a') f->mempos = strchr(buf, 0)-buf;

	f->read = mread;
	f->write = mwrite;
	f->seek = mseek;
	f->flush = mflush;
	f->close = mclose;

	return f;
}
#endif
