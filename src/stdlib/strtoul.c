#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

unsigned long strtoul(const char *s, char **p, int base)
{
	uintmax_t x = strtoumax(s, p, base);
	if (x > ULONG_MAX) {
		errno = ERANGE;
		return ULONG_MAX;
	}
	return x;
}
