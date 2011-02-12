#include <stdio.h>
#include <stdarg.h>

int snprintf(char *s, size_t n, const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vsnprintf(s, n, fmt, ap);
	va_end(ap);
	return ret;
}

