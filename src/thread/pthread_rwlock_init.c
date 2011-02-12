#include "pthread_impl.h"

int pthread_rwlock_init(pthread_rwlock_t *rw, const pthread_rwlockattr_t *a)
{
	memset(rw, 0, sizeof *rw);
	if (a) {
	}
	return 0;
}
