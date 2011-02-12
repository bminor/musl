#include "pthread_impl.h"

int pthread_mutex_init(pthread_mutex_t *m, const pthread_mutexattr_t *a)
{
	memset(m, 0, sizeof *m);
	if (a) {
	}
	return 0;
}
