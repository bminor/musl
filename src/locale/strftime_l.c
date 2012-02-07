#include <locale.h>
#include <time.h>

size_t strftime_l(char *s, size_t n, const char *f, const struct tm *tm, locale_t l)
{
	return strftime(s, n, f, tm);
}
