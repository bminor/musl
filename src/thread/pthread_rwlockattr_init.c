#include "pthread_impl.h"

int pthread_rwlockattr_init(pthread_rwlockattr_t *a)
{
	memset(a, 0, sizeof *a);
	return 0;
}
