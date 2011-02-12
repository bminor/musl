#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

unsigned long long strtoull(const char *s, char **p, int base)
{
	uintmax_t x = strtoumax(s, p, base);
	if (x > ULLONG_MAX) {
		errno = ERANGE;
		return ULLONG_MAX;
	}
	return x;
}
