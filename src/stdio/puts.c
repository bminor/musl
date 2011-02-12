#include "stdio_impl.h"

int puts(const char *s)
{
	return -(fputs(s, stdout) < 0 || putchar('\n') < 0);
}
