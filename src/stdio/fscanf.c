#include <stdio.h>
#include <stdarg.h>

int fscanf(FILE *f, const char *fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vfscanf(f, fmt, ap);
	va_end(ap);
	return ret;
}
