#include "pthread_impl.h"

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *a, int *pshared)
{
	*pshared = *(int *)a;
	return 0;
}
