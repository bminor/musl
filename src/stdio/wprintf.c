#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

int wprintf(const wchar_t *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vwprintf(fmt, ap);
	va_end(ap);
	return ret;
}
