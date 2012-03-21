#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <monetary.h>
#include <errno.h>

ssize_t strfmon(char *s, size_t n, const char *fmt, ...)
{
	size_t l;
	double x;
	int fill, nogrp, negpar, nosym, left, intl;
	int lp, rp, w, fw;
	char *s0=s;
	va_list ap;
	va_start(ap, fmt);
	for (; n && *fmt; ) {
		if (*fmt != '%') {
		literal:
			*s++ = *fmt++;
			n--;
			continue;
		}
		fmt++;
		if (*fmt == '%') goto literal;

		fill = ' ';
		nogrp = 0;
		negpar = 0;
		nosym = 0;
		left = 0;
		for (; ; fmt++) {
			switch (*fmt) {
			case '=':
				fill = *++fmt;
				continue;
			case '^':
				nogrp = 1;
				continue;
			case '(':
				negpar = 1;
			case '+':
				continue;
			case '!':
				nosym = 1;
				continue;
			case '-':
				left = 1;
				continue;
			}
			break;
		}

		for (fw=0; isdigit(*fmt); fmt++)
			fw = 10*fw + (*fmt-'0');
		lp = 0;
		rp = 2;
		if (*fmt=='#') for (lp=0, fmt++; isdigit(*fmt); fmt++)
			lp = 10*lp + (*fmt-'0');
		if (*fmt=='.') for (rp=0, fmt++; isdigit(*fmt); fmt++)
			rp = 10*rp + (*fmt-'0');

		intl = *fmt++ == 'i';

		w = lp + 1 + rp;
		if (!left && fw>w) w = fw;

		x = va_arg(ap, double);
		l = snprintf(s, n, "%*.*f", w, rp, x);
		if (l >= n) {
			errno = E2BIG;
			return -1;
		}
		s += l;
		n -= l;
	}
	return s-s0;
}
