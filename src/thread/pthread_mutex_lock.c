#include "pthread_impl.h"

int pthread_mutex_lock(pthread_mutex_t *m)
{
	return pthread_mutex_timedlock(m, 0);
}
