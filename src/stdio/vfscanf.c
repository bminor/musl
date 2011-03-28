#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "stdio_impl.h"
#include "__scanf.h"

static void f_read(rctx_t *r)
{
	FILE *f = r->opaque;
	if ((r->c = getc_unlocked(f)) >= 0) r->l++;
}

int vfscanf(FILE *f, const char *fmt, va_list ap)
{
	size_t l = strlen(fmt), i, result;
	rctx_t r = { f_read, (void *)f, 0, isspace };
	wchar_t fmt2[l+1];

	if (l > 0x100000) {
		errno = ENOMEM;
		return -1;
	}
	for (i=0; i<=l; i++) fmt2[i] = (unsigned char)fmt[i];

	FLOCK(f);

	result = __scanf(&r, fmt2, ap);

	if (r.u && r.c >= 0)
		ungetc(r.c, f);

	FUNLOCK(f);
	return result;
}
