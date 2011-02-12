#include "stdio_impl.h"

static off_t retneg1(FILE *f, off_t off, int whence)
{
	return -1;
}

off_t __stdio_seek(FILE *f, off_t off, int whence)
{
	off_t ret = __syscall_lseek(f->fd, off, whence);
	/* Detect unseekable files and optimize future failures out */
	if (ret < 0 && off == 0 && whence == SEEK_CUR)
		f->seek = retneg1;
	return ret;
}
