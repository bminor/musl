#include "stdio_impl.h"

wint_t __fputwc_unlocked(wchar_t c, FILE *f)
{
	char mbc[MB_LEN_MAX];
	int l;

	f->mode |= f->mode+1;

	if (isascii(c)) {
		if (c != f->lbf && f->wpos + 1 < f->wend) *f->wpos++ = c;
		else c = __overflow(f, c);
	} else if (f->wpos + MB_LEN_MAX < f->wend) {
		l = wctomb(f->wpos, c);
		if (l < 0) c = WEOF;
		else f->wpos += l;
	} else {
		l = wctomb(mbc, c);
		if (l < 0 || __fwritex(mbc, l, f) < l) c = WEOF;
	}
	return c;
}

wint_t fputwc(wchar_t c, FILE *f)
{
	FLOCK(f);
	c = __fputwc_unlocked(c, f);
	FUNLOCK(f);
	return 0;
}

weak_alias(__fputwc_unlocked, fputwc_unlocked);
weak_alias(__fputwc_unlocked, putwc_unlocked);
