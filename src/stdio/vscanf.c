#include <stdio.h>
#include <stdarg.h>

int vscanf(const char *fmt, va_list ap)
{
	return vfscanf(stdin, fmt, ap);
}
