#include "stdio_impl.h"
#include <wchar.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

wint_t ungetwc(wint_t c, FILE *f)
{
	unsigned char mbc[MB_LEN_MAX];
	int l=1;

	if (c == WEOF) return c;

	FLOCK(f);

	f->mode |= f->mode+1;

	if (!f->rpos) __toread(f);
	if (!f->rpos || f->rpos < f->buf - UNGET + l ||
	    (!isascii(c) && (l = wctomb((void *)mbc, c)) < 0)) {
		FUNLOCK(f);
		return WEOF;
	}

	if (isascii(c)) *--f->rpos = c;
	else memcpy(f->rpos -= l, mbc, l);

	f->flags &= ~F_EOF;

	FUNLOCK(f);
	return c;
}
