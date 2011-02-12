#include <errno.h>
#include "libc.h"

#undef errno
int errno;

int *__errno_location(void)
{
	if (libc.errno_location) return libc.errno_location();
	return &errno;
}
