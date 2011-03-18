#include <stdio.h>
#include <wchar.h>

int vwprintf(const wchar_t *fmt, va_list ap)
{
	return vfwprintf(stdout, fmt, ap);
}
