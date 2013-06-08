#include <stdlib.h>

int rand_r(unsigned *seed)
{
	return (*seed = *seed * 1103515245 + 12345)/2;
}
