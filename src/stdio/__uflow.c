#include "stdio_impl.h"

int __uflow(FILE *f)
{
	if (__underflow(f) < 0) return EOF;
	else return *f->rpos++;
}
