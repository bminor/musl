#include <wchar.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

long long wcstoll(const wchar_t *s, wchar_t **p, int base)
{
	intmax_t x = wcstoimax(s, p, base);
	if (x > LLONG_MAX) {
		errno = ERANGE;
		return LLONG_MAX;
	} else if (x < LLONG_MIN) {
		errno = ERANGE;
		return LLONG_MIN;
	}
	return x;
}
