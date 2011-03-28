#include "stdio_impl.h"

static off_t retneg1(FILE *f, off_t off, int whence)
{
	errno = ESPIPE;
	return -1;
}

off_t __stdio_seek(FILE *f, off_t off, int whence)
{
	off_t ret;
#ifdef SYS__llseek
	if (syscall(SYS__llseek, f->fd, off>>32, off, &ret, whence)<0)
		ret = -1;
#else
	ret = syscall(SYS_lseek, f->fd, off, whence);
#endif
	/* Detect unseekable files and optimize future failures out */
	if (ret < 0 && errno == ESPIPE) f->seek = retneg1;
	return ret;
}
