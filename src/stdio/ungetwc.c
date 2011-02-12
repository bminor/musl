#include "stdio_impl.h"

wint_t ungetwc(wint_t c, FILE *f)
{
	unsigned char mbc[MB_LEN_MAX];
	int l=1;

	if (c == WEOF) return c;

	/* Try conversion early so we can fail without locking if invalid */
	if (!isascii(c) && (l = wctomb(mbc, c)) < 0)
		return WEOF;

	FLOCK(f);

	f->mode |= f->mode+1;

	/* Fail if unreadable or writing and unable to flush */
	if ((f->flags & (F_ERR|F_NORD)) || (f->wpos && __oflow(f))) {
		FUNLOCK(f);
		return EOF;
	}

	/* Clear write mode */
	f->wpos = f->wstop = f->wend = 0;

	/* Put the file in read mode */
	if (!f->rpos) f->rpos = f->rend = f->buf;

	/* If unget buffer is nonempty, fail. */
	if (f->rpos < f->buf) {
		FUNLOCK(f);
		return WEOF;
	}

	/* Put character back into the buffer */
	if (isascii(c)) *--f->rpos = c;
	else memcpy(f->rpos -= l, mbc, l);

	/* Clear EOF */
	f->flags &= ~F_EOF;

	FUNLOCK(f);
	return c;
}
