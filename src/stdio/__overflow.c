#include "stdio_impl.h"

static int overflow(FILE *f, int c)
{
	/* Initialize if we're not already writing */
	if (!f->wend) {
		/* Fail if we're in error state or unwritable. */
		if (f->flags & (F_ERR|F_NOWR)) return EOF;

		/* Set byte orientation -1,0=>-1; 1=>1 */
		f->mode |= f->mode-1;

		/* Clear read buffer (easier than summoning nasal demons) */
		f->rpos = f->rend = f->rstop = 0;

		/* Activate write through the buffer */
		f->wpos = f->wbase = f->buf;
		f->wend = f->buf + f->buf_size;
		f->wstop = (f->lbf < 0) ? f->wend - 1 : 0;
	}

	/* Buffer can always hold at least 1 byte... */
	if (c != EOF) {
		*f->wpos++ = c;
		if (f->wpos <= f->wstop && c != f->lbf) return c;
	}
	/* ...since if the next call fails, buffer is empty. */
	if (f->write(f, f->wbase, f->wpos - f->wbase) < 0) {
		f->flags |= F_ERR;
		f->wpos = f->wbase = f->wend = f->wstop = 0;
		return EOF;
	}

	/* Buffer is empty so reset position to beginning */
	f->wpos = f->wbase;

	return c;
}

int __overflow(FILE *f, int c)
{
	return overflow(f, c & 0xff);
}

int __oflow(FILE *f)
{
	overflow(f, EOF);
	return (f->flags & F_ERR) ? EOF : 0;
}

/* Link flush-on-exit code iff any stdio write functions are linked. */
int (*const __fflush_on_exit)(FILE *) = fflush;
