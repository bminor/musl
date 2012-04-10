#include <stdlib.h>
#include "floatscan.h"
#include "stdio_impl.h"

double strtod(const char *s, char **p)
{
	FILE f = {
		.buf = (void *)s, .rpos = (void *)s,
		.rend = (void *)-1, .lock = -1
	};
	off_t cnt;
	double y = __floatscan(&f, -1, 1, 1, &cnt);
	if (p) *p = (char *)s + cnt;
	return y;
}
