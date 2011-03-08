#include "pthread_impl.h"

int pthread_mutexattr_setpshared(pthread_mutexattr_t *a, int pshared)
{
	if (pshared > 1U) return EINVAL;
	*a &= 0x7fffffff;
	*a |= pshared<<31;
	return 0;
}
