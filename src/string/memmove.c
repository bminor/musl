#include <string.h>

void *memmove(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;
	if (d==s) return d;
	if ((size_t)(d-s) < n) {
		while (n--) d[n] = s[n];
		return dest;
	}
	/* Assumes memcpy is overlap-safe when dest < src */
	return memcpy(d, s, n);
}
