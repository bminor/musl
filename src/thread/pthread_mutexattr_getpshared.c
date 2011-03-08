#include "pthread_impl.h"

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *a, int *pshared)
{
	*pshared = *a>>31;
	return 0;
}
