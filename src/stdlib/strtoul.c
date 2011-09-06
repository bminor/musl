#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

unsigned long strtoul(const char *s, char **p, int base)
{
	intmax_t x;
	if (sizeof(intmax_t) == sizeof(long))
		return strtoumax(s, p, base);
	x = strtoimax(s, p, base);
	if (-x > ULONG_MAX || x > ULONG_MAX) {
		errno = ERANGE;
		return ULONG_MAX;
	}
	return x;
}
