#include <errno.h>
#include "libc.h"

int __clone(int (*func)(void *), void *stack, int flags, void *arg, ...)
{
	errno = ENOSYS;
	return -1;
}

weak_alias(__clone, clone);
