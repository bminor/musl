#include <wchar.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

unsigned long wcstoul(const wchar_t *s, wchar_t **p, int base)
{
 	intmax_t x;
	if (sizeof(intmax_t) == sizeof(long))
		return wcstoumax(s, p, base);
	x = wcstoimax(s, p, base);
	if (-x > ULONG_MAX || x > ULONG_MAX) {
		errno = ERANGE;
		return ULONG_MAX;
	}
	return x;
}
