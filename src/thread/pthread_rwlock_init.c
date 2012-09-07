#include "pthread_impl.h"

int pthread_rwlock_init(pthread_rwlock_t *restrict rw, const pthread_rwlockattr_t *restrict a)
{
	memset(rw, 0, sizeof *rw);
	if (a) {
	}
	return 0;
}
