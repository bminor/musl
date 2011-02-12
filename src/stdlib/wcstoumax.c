#include <wchar.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>

uintmax_t wcstoumax(const wchar_t *s, wchar_t **p, int base)
{
	/* Large enough for largest value in binary */
	char buf[sizeof(uintmax_t)*8+2];
	int sign = 0, skipped=0;

	if (!p) p = (wchar_t **)&s;

	if (base && (unsigned)base-2 > 36-2) {
		*p = (wchar_t *)s;
		errno = EINVAL;
		return 0;
	}

	/* Initial whitespace */
	for (; iswspace(*s); s++);

	/* Optional sign */
	if (*s == '-') sign = *s++;
	else if (*s == '+') s++;

	/* Skip leading zeros but don't allow leading zeros before "0x". */
	for (; s[0]=='0' && s[1]=='0'; s++) skipped=1;
	if (skipped && (base==0 || base==16) && (s[1]|32)=='x') {
		*p = (wchar_t *)(s+1);
		return 0;
	}

	/* Convert to normal char string so we can use strtoumax */
	buf[0] = sign;
	if (wcstombs(buf+!!sign, s, sizeof buf-1) < 0) return 0;
	buf[sizeof buf-1]=0;

	/* Compute final position */
	if (p) {
		if ((base==0 || base==16) && s[0]=='0' && (s[1]|32)=='x' && iswxdigit(s[2])) s+=2;
		for(;*s&&((unsigned)*s-'0'<base||((unsigned)*s|32)-'a'<base-10);s++);
		*p = (wchar_t *)s;
	}

	return strtoumax(buf, 0, base);
}
