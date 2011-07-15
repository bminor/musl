#include <inttypes.h>
#include <errno.h>
#include <ctype.h>
#include "intparse.h"

intmax_t strtoimax(const char *s1, char **p, int base)
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
		if (ip.err == EINVAL) return 0;
		return ip.neg ? INTMAX_MIN : INTMAX_MAX;
	}

	if (ip.val > INTMAX_MAX) {
		if (!ip.neg || -ip.val != INTMAX_MIN)
			errno = ERANGE;
		return ip.neg ? INTMAX_MIN : INTMAX_MAX;
	}
	return ip.neg ? -ip.val : ip.val;
}
