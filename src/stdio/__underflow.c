#include "stdio_impl.h"

int __underflow(FILE *f)
{
	ssize_t cnt;

	/* Read from buffer (Do we ever get called when this is true??) */
	if (f->rpos < f->rstop) return *f->rpos;

	/* Initialize if we're not already reading */
	if (!f->rstop) {
		/* Fail immediately if unreadable, eof, or error state. */
		if (f->flags & (F_EOF|F_ERR|F_NORD)) return EOF;

		/* Set byte orientation -1,0=>-1; 1=>1 */
		f->mode |= f->mode-1;

		/* Flush any unwritten output; fail on error. */
		if (f->wpos > f->buf && __oflow(f)) return EOF;

		/* Disallow writes to buffer. */
		f->wstop = 0;
	}

	/* Perform the underlying read operation */
	if ((cnt=f->read(f, f->buf, f->buf_size)) + 1 <= 1) {
		/* Set flags and leave read mode */
		f->flags |= F_EOF | (cnt & F_ERR);
		f->rpos = f->rend = f->rstop = 0;
		return EOF;
	}

	/* Setup buffer pointers for reading from buffer */
	f->rpos = f->buf;
	f->rend = f->rstop = f->buf + cnt;

	return *f->rpos;
}
