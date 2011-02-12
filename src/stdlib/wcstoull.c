#include <wchar.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

unsigned long long wcstoull(const wchar_t *s, wchar_t **p, int base)
{
	uintmax_t x = wcstoumax(s, p, base);
	if (x > ULLONG_MAX) {
		errno = ERANGE;
		return ULLONG_MAX;
	}
	return x;
}
