#include "pthread_impl.h"

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict a, int *restrict pshared)
{
	*pshared = *a>>31;
	return 0;
}
