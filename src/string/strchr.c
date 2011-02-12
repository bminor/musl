#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

char *strchr(const char *s, int c)
{
	c = (char)c;
	if (!c) return (char *)s + strlen(s);
	for (; ((uintptr_t)s & ALIGN) && *s && *s != c; s++);
	if (*s && *s != c) {
		const size_t *w;
		size_t k = ONES * c;
		for (w = (const void *)s; !HASZERO(*w) && !HASZERO(*w^k); w++);
		for (s = (const void *)w; *s && *s != c; s++);
	}
	return *s ? (char *)s : 0;
}
