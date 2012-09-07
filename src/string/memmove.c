#include <string.h>

void *memmove(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;
	if (d==s) return d;
	if ((size_t)(d-s) < n)
		while (n--) d[n] = s[n];
	else
		while (n--) *d++ = *s++;
	return dest;
}
