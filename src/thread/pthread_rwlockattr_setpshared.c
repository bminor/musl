#include "pthread_impl.h"

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *a, int pshared)
{
	if (pshared > 1U) return EINVAL;
	*(int *)a = pshared;
	return 0;
}
