#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

long strtol(const char *s, char **p, int base)
{
	intmax_t x = strtoimax(s, p, base);
	if (x > LONG_MAX) {
		errno = ERANGE;
		return LONG_MAX;
	} else if (x < LONG_MIN) {
		errno = ERANGE;
		return LONG_MIN;
	}
	return x;
}
