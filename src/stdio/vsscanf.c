#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "__scanf.h"

static void s_read(rctx_t *r)
{
	unsigned char *s = r->opaque;
	if (!s[r->l]) r->c = -1;
	else r->c = s[r->l++];
}

int vsscanf(const char *s, const char *fmt, va_list ap)
{
	size_t l = strlen(fmt), i;
	wchar_t fmt2[l+1];
	rctx_t r = { s_read, (void *)s, 0, isspace };
	for (i=0; i<=l; i++) fmt2[i] = (unsigned char)fmt[i];
	return __scanf(&r, fmt2, ap);
}
