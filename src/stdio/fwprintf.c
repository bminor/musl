#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

int fwprintf(FILE *f, const wchar_t *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vfwprintf(f, fmt, ap);
	va_end(ap);
	return ret;
}
