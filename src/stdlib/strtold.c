#include <stdlib.h>
#include "floatscan.h"
#include "stdio_impl.h"

long double strtold(const char *s, char **p)
{
	FILE f = {
		.buf = (void *)s, .rpos = (void *)s,
		.rend = (void *)-1, .lock = -1
	};
	off_t cnt;
	long double y = __floatscan(&f, -1, 2, 1, &cnt);
	if (p) *p = (char *)s + cnt;
	return y;
}
