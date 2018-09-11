#define _BSD_SOURCE
#include <stdlib.h>
#include "libc.h"

int mkostemp(char *template, int flags)
{
	return __mkostemps(template, 0, flags);
}

LFS64(mkostemp);
