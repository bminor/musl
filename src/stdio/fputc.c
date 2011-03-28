#include "stdio_impl.h"

int fputc(int c, FILE *f)
{
	FLOCK(f);
	c = putc_unlocked(c, f);
	FUNLOCK(f);
	return c;
}

weak_alias(fputc, putc);
