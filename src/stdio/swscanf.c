#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

int swscanf(const wchar_t *s, const wchar_t *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vswscanf(s, fmt, ap);
	va_end(ap);
	return ret;
}
