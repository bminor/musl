#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

long long strtoll(const char *s, char **p, int base)
{
	intmax_t x = strtoimax(s, p, base);
	if (x > LLONG_MAX) {
		errno = ERANGE;
		return LLONG_MAX;
	} else if (x < LLONG_MIN) {
		errno = ERANGE;
		return LLONG_MIN;
	}
	return x;
}
