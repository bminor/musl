#include "stdio_impl.h"

int puts(const char *s)
{
	int r;
	FLOCK(stdout);
	r = -(fputs(s, stdout) < 0 || putchar('\n') < 0);
	FUNLOCK(stdout);
	return r;
}
