#include <wchar.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

long wcstol(const wchar_t *s, wchar_t **p, int base)
{
	intmax_t x = wcstoimax(s, p, base);
	if (x > LONG_MAX) {
		errno = ERANGE;
		return LONG_MAX;
	} else if (x < LONG_MIN) {
		errno = ERANGE;
		return LONG_MIN;
	}
	return x;
}
