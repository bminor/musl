#include "stdio_impl.h"

int fgetc(FILE *f)
{
	int c;
	if (f->lock < 0 || !__lockfile(f))
		return getc_unlocked(f);
	c = getc_unlocked(f);
	__unlockfile(f);
	return c;
}

weak_alias(fgetc, getc);
weak_alias(fgetc, _IO_getc);
