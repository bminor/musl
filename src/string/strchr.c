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
	size_t *w, k;

	c = (unsigned char)c;
	if (!c) return (char *)s + strlen(s);

	for (; ((uintptr_t)s & ALIGN); s++)
		if (*(unsigned char *)s == c) return (char *)s;
		else if (!*s) return 0;
	k = ONES * c;
	for (w = (void *)s; !HASZERO(*w) && !HASZERO(*w^k); w++);
	for (s = (void *)w; *s; s++)
		if (*(unsigned char *)s == c) return (char *)s;
	return 0;
}
