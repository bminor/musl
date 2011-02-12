#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

long double strtold(const char *s1, char **p)
{
	const unsigned char *s = s1;
	long double x = 0;
	long double frac;
	int sign = 0;
	int nonzero = 0;
	int radix = '.';
	long e;

	if (!p) p = (char **)&s1;

	/* Initial whitespace */
	for (; isspace(*s); s++);

	/* Optional sign */
	if (*s == '-') sign = *s++;
	else if (*s == '+') s++;

	/* Handle infinities and NaNs. */
	if ((s[0]|32)=='i' && (s[1]|32)=='n' && (s[2]|32)=='f') {
		*p = (char *)s + 3;
		return sign ? -1.0/0.0 : 1.0/0.0;
	} else if ((s[0]|32)=='n' && (s[1]|32)=='a' && (s[2]|32)=='n') {
		*p = (char *)s + 3;
		return 0.0/0.0;
	}

	/* Possible hex float */
	if (s[0]=='0' && (s[1]|32)=='x') {
		/* Mantissa must be non-degenerate */
		if (!isxdigit(s[2]) && (s[2]!=radix || !isxdigit(s[3]))) {
			/* Decimal float 0, 'x' extraneous */
			*p = (char *)++s;
			return 0;
		}
		/* We have a real hex float */
		s += 2;
		for (; isxdigit(*s); s++) {
			x = 16*x + (isdigit(*s)?*s-'0':(*s|32)-'a');
			if (*s!='0') nonzero=1;
		}
		if (*s == radix) {
			frac = 1.0/16.0;
			for (s++; isxdigit(*s); s++) {
				x += frac * (isdigit(*s)?*s-'0':(*s|32)-'a');
				frac *= 1.0/16.0;
				if (*s!='0') nonzero=1;
			}
		}
		if ((*s|32) == 'p') {
			e = strtol(s+1, (void *)&s, 10);
			for (; e>0; e--) x *= 2.0;
			for (; e<0; e++) x *= 0.5;
		}
		if ((nonzero && !x) || !(1.0/x))
			errno = ERANGE;
		*p = (char *)s;
		return sign ? -x : x;
	}

	/* Mantissa must be non-degenerate */
	if (!isdigit(s[0]) && (s[0]!=radix || !isdigit(s[1]))) {
		*p = (char *)s1;
		return 0;
	}

	for (; isdigit(*s); s++) {
		x = 10*x + *s-'0';
		if (*s!='0') nonzero=1;
	}
	if (*s == radix) {
		frac = 10.0;
		for (s++; isdigit(*s); s++) {
			x += (*s-'0') / frac;
			frac *= 10.0;
			if (*s!='0') nonzero=1;
		}
	}
	if ((*s|32)=='e') {
		e = strtol(++s, (void *)&s, 10);
		for (; e>0; e--) x *= 10.0;
		for (; e<0; e++) x /= 10.0;
	}
	if ((nonzero && !x) || !(1.0/x))
		errno = ERANGE;
	*p = (char*)s;
	return sign ? -x : x;
}
