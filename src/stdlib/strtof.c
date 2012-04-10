#include <stdlib.h>
#include "floatscan.h"
#include "stdio_impl.h"

float strtof(const char *s, char **p)
{
	FILE f = {
		.buf = (void *)s, .rpos = (void *)s,
		.rend = (void *)-1, .lock = -1
	};
	off_t cnt;
	float y = __floatscan(&f, -1, 0, 1, &cnt);
	if (p) *p = (char *)s + cnt;
	return y;
}
