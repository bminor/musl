#include "pthread_impl.h"

int pthread_cond_wait(pthread_cond_t *c, pthread_mutex_t *m)
{
	return pthread_cond_timedwait(c, m, 0);
}
