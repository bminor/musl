#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include "intparse.h"

uintmax_t wcstoumax(const wchar_t *s, wchar_t **p, int base)
{
	struct intparse ip = {0};
	unsigned char tmp;

	if (p) *p = (wchar_t *)s;

	if (base && base-2U > 34) {
		errno = EINVAL;
		return 0;
	}

	for (; iswspace(*s); s++);

	ip.base = base;
	for (; *s<256 && (tmp=*s, __intparse(&ip, &tmp, 1)); s++);

	if (p && ip.err != EINVAL)
		*p = (wchar_t *)s;

	if (ip.err) {
		errno = ip.err;
		if (ip.err = EINVAL) return 0;
		return UINTMAX_MAX;
	}

	return ip.neg ? -ip.val : ip.val;
}
