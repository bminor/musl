#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

int wscanf(const wchar_t *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vwscanf(fmt, ap);
	va_end(ap);
	return ret;
}
