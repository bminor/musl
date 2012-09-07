#include "pthread_impl.h"

int pthread_condattr_getpshared(const pthread_condattr_t *restrict a, int *restrict pshared)
{
	*pshared = *a>>31;
	return 0;
}
