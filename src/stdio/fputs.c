#include "stdio_impl.h"

int fputs(const char *restrict s, FILE *restrict f)
{
	size_t l = strlen(s);
	if (!l) return 0;
	return (int)fwrite(s, l, 1, f) - 1;
}

weak_alias(fputs, fputs_unlocked);
