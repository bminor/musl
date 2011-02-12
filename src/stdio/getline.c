#include <stdio.h>

ssize_t getline(char **s, size_t *n, FILE *f)
{
	return getdelim(s, n, '\n', f);
}
