#include <wchar.h>
#include <inttypes.h>
#include <errno.h>

intmax_t wcstoimax(const wchar_t *s, wchar_t **p, int base)
{
	int sign = 0;
	uintmax_t x;

	/* Initial whitespace */
	for (; iswspace(*s); s++);

	/* Optional sign */
	if (*s == '-') sign = *s++;
	else if (*s == '+') s++;

	x = wcstoumax(s, p, base);
	if (x > INTMAX_MAX) {
		if (!sign || -x != INTMAX_MIN)
			errno = ERANGE;
		return sign ? INTMAX_MIN : INTMAX_MAX;
	}
	return sign ? -x : x;
}
