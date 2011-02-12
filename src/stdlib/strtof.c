#include <stdlib.h>

float strtof(const char *s, char **p)
{
	return strtold(s, p);
}
