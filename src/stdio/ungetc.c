#include "stdio_impl.h"

int ungetc(int c, FILE *f)
{
	if (c == EOF) return c;

	FLOCK(f);

	/* Fail if unreadable or writing and unable to flush */
	if ((f->flags & (F_ERR|F_NORD)) || (f->wpos && __oflow(f))) {
		FUNLOCK(f);
		return EOF;
	}

	/* Clear write mode */
	f->wbase = f->wpos = f->wstop = f->wend = 0;

	/* Put the file in read mode */
	if (!f->rpos) f->rpos = f->rend = f->buf;

	/* If unget buffer is already full, fail. */
	if (f->rpos <= f->buf - UNGET) {
		FUNLOCK(f);
		return EOF;
	}

	/* Put a byte back into the buffer */
	*--f->rpos = c;
	f->flags &= ~F_EOF;

	FUNLOCK(f);
	return c;
}
