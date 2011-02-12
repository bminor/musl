#include <stdio.h>
#include <stdarg.h>

int sprintf(char *s, const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsprintf(s, fmt, ap);
	va_end(ap);
	return ret;
}
