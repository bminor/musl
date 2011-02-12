#include <strings.h>

int ffs(int i)
{
	unsigned int j = i;
	for (i=1; j && !(j&1); j>>=1, i++);
	if (j) return i;
	return 0;
}
