#include "stdio_impl.h"

static unsigned char buf[BUFSIZ+UNGET];
static FILE f = {
	.buf = buf+UNGET,
	.buf_size = sizeof buf-UNGET,
	.fd = 1,
	.flags = F_PERM | F_NORD,
	.lbf = '\n',
	.write = __stdio_write,
	.seek = __stdio_seek,
	.close = __stdio_close,
};
FILE *const stdout = &f;

/* overrides symbol in fflush.c, used for flushing NULL */
FILE *const __stdout_to_flush = &f;
