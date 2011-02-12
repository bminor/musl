#include "stdio_impl.h"

int putc(int c, FILE *f)
{
	return fputc(c, f);
}

weak_alias(putc, _IO_putc);
