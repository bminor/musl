#include <string.h>

char *strncat(char *d, const char *s, size_t n)
{
	char *a = d;
	d += strlen(d);
	while (n && (*d++ = *s++)) n--;
	*d++ = 0;
	return a;
}
