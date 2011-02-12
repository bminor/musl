#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

int vwscanf(const wchar_t *fmt, va_list ap)
{
	return vfwscanf(stdin, fmt, ap);
}
