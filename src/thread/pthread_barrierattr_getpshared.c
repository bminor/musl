#include "pthread_impl.h"

int pthread_barrierattr_getpshared(const pthread_barrierattr_t *a, int *pshared)
{
	*pshared = !!*a;
	return 0;
}
