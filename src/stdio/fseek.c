#include "stdio_impl.h"

int __fseeko_unlocked(FILE *f, off_t off, int whence)
{
	/* Adjust relative offset for unread data in buffer, if any. */
	if (whence == SEEK_CUR) off -= f->rend - f->rpos;

	/* If writing, flush output. */
	if (f->wpos > f->buf && __oflow(f)) return -1;

	/* Perform the underlying seek operation. */
	if (f->seek(f, off, whence) < 0) return -1;

	/* If seek succeeded, file is seekable and we discard read buffer. */
	f->rpos = f->rend = f->rstop = 0;
	f->flags &= ~F_EOF;
	
	FUNLOCK(f);	
	return 0;
}

int __fseeko(FILE *f, off_t off, int whence)
{
	int result;
	FLOCK(f);
	result = __fseeko_unlocked(f, off, whence);
	FUNLOCK(f);
	return result;
}

int fseek(FILE *f, long off, int whence)
{
	return __fseeko(f, off, whence);
}

weak_alias(__fseeko, fseeko);

LFS64(fseeko);
