#include <string.h>

char *__stpncpy(char *, const char *, size_t);

char *strncpy(char *d, const char *s, size_t n)
{
	__stpncpy(d, s, n);
	return d;
}
