#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "intparse.h"

uintmax_t strtoumax(const char *s1, char **p, int base)
{
	const unsigned char *s = (void *)s1;
	struct intparse ip = {0};

	if (p) *p = (char *)s1;

	if (base && base-2U > 34) {
		errno = EINVAL;
		return 0;
	}

	for (; isspace(*s); s++);

	ip.base = base;
	__intparse(&ip, s, SIZE_MAX);

	if (p && ip.err != EINVAL)
		*p = (char *)s + ip.cnt;

	if (ip.err) {
		errno = ip.err;
		if (ip.err = EINVAL) return 0;
		return UINTMAX_MAX;
	}

	return ip.neg ? -ip.val : ip.val;
}
