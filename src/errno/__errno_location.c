#include <errno.h>
#include "libc.h"

int *__errno_location(void)
{
	static int e;
	if (libc.errno_location) return libc.errno_location();
	return &e;
}
