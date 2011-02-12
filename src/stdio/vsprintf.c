#include <stdio.h>
#include <limits.h>

int vsprintf(char *s, const char *fmt, va_list ap)
{
	return vsnprintf(s, INT_MAX, fmt, ap);
}
