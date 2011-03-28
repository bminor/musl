#include "stdio_impl.h"

int fgetc(FILE *f)
{
	int c;
	FLOCK(f);
	c = getc_unlocked(f);
	FUNLOCK(f);
	return c;
}

weak_alias(fgetc, getc);
