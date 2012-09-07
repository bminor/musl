#include "pthread_impl.h"

int pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict a, int *restrict pshared)
{
	*pshared = !!*a;
	return 0;
}
