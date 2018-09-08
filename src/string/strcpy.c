#include <string.h>

char *__stpcpy(char *, const char *);

char *strcpy(char *restrict dest, const char *restrict src)
{
	__stpcpy(dest, src);
	return dest;
}
