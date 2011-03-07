#include "pthread_impl.h"

int pthread_condattr_getpshared(const pthread_condattr_t *a, int *pshared)
{
	*pshared = *a>>31;
	return 0;
}
