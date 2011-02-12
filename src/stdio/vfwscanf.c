#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>
#include <wctype.h>

#include "stdio_impl.h"
#include "__scanf.h"

static void f_read(rctx_t *r)
{
	FILE *f = r->opaque;
	if ((r->c = fgetwc(f)) >= 0) r->l++;
}

int vfwscanf(FILE *f, const wchar_t *fmt, va_list ap)
{
	rctx_t r = { f_read, (void *)f, 1, iswspace };
	int result;

	result = __scanf(&r, fmt, ap);

	if (r.u && r.c >= 0) {
		ungetwc(r.c, f);
	}

	return result;
}
