#include <stdio.h>
#include <stdlib.h>
#include <langinfo.h>
#include <locale.h>
#include <time.h>
#include <limits.h>
#include "libc.h"

// FIXME: integer overflows

const char *__nl_langinfo_l(nl_item, locale_t);

static int is_leap(int y)
{
	/* Avoid overflow */
	if (y>INT_MAX-1900) y -= 2000;
	y += 1900;
	return !(y%4) && ((y%100) || !(y%400));
}

static int week_num(const struct tm *tm)
{
	int val = (tm->tm_yday + 7 - (tm->tm_wday+6)%7) / 7;
	/* If 1 Jan is just 1-3 days past Monday,
	 * the previous week is also in this year. */
	if ((tm->tm_wday - tm->tm_yday - 2 + 371) % 7 <= 2)
		val++;
	if (!val) {
		val = 52;
		/* If 31 December of prev year a Thursday,
		 * or Friday of a leap year, then the
		 * prev year has 53 weeks. */
		int dec31 = (tm->tm_wday - tm->tm_yday - 1 + 7) % 7;
		if (dec31 == 4 || (dec31 == 5 && is_leap(tm->tm_year%400-1)))
			val++;
	} else if (val == 53) {
		/* If 1 January is not a Thursday, and not
		 * a Wednesday of a leap year, then this
		 * year has only 52 weeks. */
		int jan1 = (tm->tm_wday - tm->tm_yday + 371) % 7;
		if (jan1 != 4 && (jan1 != 3 || !is_leap(tm->tm_year)))
			val = 1;
	}
	return val;
}

size_t __strftime_l(char *restrict s, size_t n, const char *restrict f, const struct tm *restrict tm, locale_t loc)
{
	nl_item item;
	int val;
	const char *fmt;
	size_t l;
	for (l=0; *f && l<n; f++) {
		if (*f != '%') {
literal:
			s[l++] = *f;
			continue;
		}
do_fmt:
		switch (*++f) {
		case '%':
			goto literal;
		case 'E':
		case 'O':
			goto do_fmt;
		case 'a':
			item = ABDAY_1 + tm->tm_wday;
			goto nl_strcat;
		case 'A':
			item = DAY_1 + tm->tm_wday;
			goto nl_strcat;
		case 'h':
		case 'b':
			item = ABMON_1 + tm->tm_mon;
			goto nl_strcat;
		case 'B':
			item = MON_1 + tm->tm_mon;
			goto nl_strcat;
		case 'c':
			item = D_T_FMT;
			goto nl_strftime;
		case 'C':
			val = (1900+tm->tm_year) / 100;
			fmt = "%02d";
			goto number;
		case 'd':
			val = tm->tm_mday;
			fmt = "%02d";
			goto number;
		case 'D':
			fmt = "%m/%d/%y";
			goto recu_strftime;
		case 'e':
			val = tm->tm_mday;
			fmt = "%2d";
			goto number;
		case 'F':
			fmt = "%Y-%m-%d";
			goto recu_strftime;
		case 'g':
		case 'G':
			fmt = "%04d";
			val = tm->tm_year + 1900;
			if (tm->tm_yday < 3 && week_num(tm) != 1) val--;
			else if (tm->tm_yday > 360 && week_num(tm) == 1) val++;
			if (*f=='g') {
				fmt = "%02d";
				val %= 100;
			}
			goto number;
		case 'H':
			val = tm->tm_hour;
			fmt = "%02d";
			goto number;
		case 'I':
			val = tm->tm_hour;
			if (!val) val = 12;
			else if (val > 12) val -= 12;
			fmt = "%02d";
			goto number;
		case 'j':
			val = tm->tm_yday+1;
			fmt = "%03d";
			goto number;
		case 'm':
			val = tm->tm_mon+1;
			fmt = "%02d";
			goto number;
		case 'M':
			val = tm->tm_min;
			fmt = "%02d";
			goto number;
		case 'n':
			s[l++] = '\n';
			continue;
		case 'p':
			item = tm->tm_hour >= 12 ? PM_STR : AM_STR;
			goto nl_strcat;
		case 'r':
			item = T_FMT_AMPM;
			goto nl_strftime;
		case 'R':
			fmt = "%H:%M";
			goto recu_strftime;
		case 'S':
			val = tm->tm_sec;
			fmt = "%02d";
			goto number;
		case 't':
			s[l++] = '\t';
			continue;
		case 'T':
			fmt = "%H:%M:%S";
			goto recu_strftime;
		case 'u':
			val = tm->tm_wday ? tm->tm_wday : 7;
			fmt = "%d";
			goto number;
		case 'U':
			val = (tm->tm_yday + 7 - tm->tm_wday) / 7;
			fmt = "%02d";
			goto number;
		case 'W':
			val = (tm->tm_yday + 7 - (tm->tm_wday+6)%7) / 7;
			fmt = "%02d";
			goto number;
		case 'V':
			val = week_num(tm);
			fmt = "%02d";
			goto number;
		case 'w':
			val = tm->tm_wday;
			fmt = "%d";
			goto number;
		case 'x':
			item = D_FMT;
			goto nl_strftime;
		case 'X':
			item = T_FMT;
			goto nl_strftime;
		case 'y':
			val = tm->tm_year % 100;
			fmt = "%02d";
			goto number;
		case 'Y':
			val = tm->tm_year + 1900;
			fmt = "%04d";
			goto number;
		case 'z':
			val = -tm->__tm_gmtoff;
			l += snprintf(s+l, n-l, "%+.2d%.2d", val/3600, abs(val%3600)/60);
			continue;
		case 'Z':
			l += snprintf(s+l, n-l, "%s", tm->__tm_zone);
			continue;
		default:
			return 0;
		}
number:
		l += snprintf(s+l, n-l, fmt, val);
		continue;
nl_strcat:
		l += snprintf(s+l, n-l, "%s", __nl_langinfo_l(item, loc));
		continue;
nl_strftime:
		fmt = __nl_langinfo_l(item, loc);
recu_strftime:
		l += __strftime_l(s+l, n-l, fmt, tm, loc);
	}
	if (l >= n) return 0;
	s[l] = 0;
	return l;
}

size_t strftime(char *restrict s, size_t n, const char *restrict f, const struct tm *restrict tm)
{
	return __strftime_l(s, n, f, tm, 0);
}

weak_alias(__strftime_l, strftime_l);
