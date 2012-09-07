#include "stdio_impl.h"

/* The basic idea of this implementation is to open a new FILE,
 * hack the necessary parts of the new FILE into the old one, then
 * close the new FILE. */

/* Locking is not necessary because, in the event of failure, the stream
 * passed to freopen is invalid as soon as freopen is called. */

FILE *freopen(const char *restrict filename, const char *restrict mode, FILE *restrict f)
{
	int fl;
	FILE *f2;

	fflush(f);

	if (!filename) {
		f2 = fopen("/dev/null", mode);
		if (!f2) goto fail;
		fl = syscall(SYS_fcntl, f2->fd, F_GETFL, 0);
		if (fl < 0 || syscall(SYS_fcntl, f->fd, F_SETFL, fl) < 0)
			goto fail2;
	} else {
		f2 = fopen(filename, mode);
		if (!f2) goto fail;
		if (syscall(SYS_dup2, f2->fd, f->fd) < 0)
			goto fail2;
	}

	f->flags = (f->flags & F_PERM) | f2->flags;
	f->read = f2->read;
	f->write = f2->write;
	f->seek = f2->seek;
	f->close = f2->close;

	fclose(f2);
	return f;

fail2:
	fclose(f2);
fail:
	fclose(f);
	return NULL;
}

LFS64(freopen);
