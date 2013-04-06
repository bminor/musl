#include <stdio.h>
#include <stdarg.h>
#include "libc.h"

int fscanf(FILE *restrict f, const char *restrict fmt, ...)
{
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vfscanf(f, fmt, ap);
	va_end(ap);
	return ret;
}

weak_alias(__isoc99_fscanf);
