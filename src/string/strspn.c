#include <string.h>

#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

size_t strspn(const char *_s, const char *_c)
{
	const unsigned char *s = _s;
	const unsigned char *c = _c;
	const unsigned char *a = s;
	size_t byteset[32/sizeof(size_t)] = { 0 };

	if (!c[0]) return 0;
	if (!c[1]) {
		for (; *s == *c; s++);
		return s-a;
	}

	for (; *c && BITOP(byteset, *c, |=); c++);
	for (; *s && BITOP(byteset, *s, &); s++);
	return s-a;
}
