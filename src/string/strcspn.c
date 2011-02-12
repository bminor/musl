#include <string.h>

#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

size_t strcspn(const char *_s, const char *_c)
{
	const unsigned char *s = _s;
	const unsigned char *c = _c;
	const unsigned char *a = s;
	size_t byteset[32/sizeof(size_t)];

	if (!c[0]) return strlen(s);
	if (!c[1]) return (s=strchr(s, *c)) ? s-a : strlen(a);

	memset(byteset, 0, sizeof byteset);
	for (; *c && BITOP(byteset, *c, |=); c++);
	for (; *s && !BITOP(byteset, *s, &); s++);
	return s-a;
}
