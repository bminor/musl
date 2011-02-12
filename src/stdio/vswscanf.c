#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#include "__scanf.h"

static void s_read(rctx_t *r)
{
	wchar_t *s = r->opaque;
	if (!s[r->l]) r->c = -1;
	else r->c = s[r->l++];
}

int vswscanf(const wchar_t *s, const wchar_t *fmt, va_list ap)
{
	rctx_t r = { s_read, (void *)s, 1, iswspace };
	return __scanf(&r, fmt, ap);
}
