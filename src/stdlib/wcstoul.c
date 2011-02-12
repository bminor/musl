#include <wchar.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

unsigned long wcstoul(const wchar_t *s, wchar_t **p, int base)
{
	uintmax_t x = wcstoumax(s, p, base);
	if (x > ULONG_MAX) {
		errno = ERANGE;
		return ULONG_MAX;
	}
	return x;
}
