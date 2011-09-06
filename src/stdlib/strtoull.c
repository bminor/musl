#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

unsigned long long strtoull(const char *s, char **p, int base)
{
	intmax_t x;
	if (sizeof(intmax_t) == sizeof(long long))
		return strtoumax(s, p, base);
	x = strtoimax(s, p, base);
	if (-x > ULLONG_MAX || x > ULLONG_MAX) {
		errno = ERANGE;
		return ULLONG_MAX;
	}
	return x;
}
