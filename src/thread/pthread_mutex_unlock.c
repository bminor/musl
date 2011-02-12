#include "pthread_impl.h"

int pthread_mutex_unlock(pthread_mutex_t *m)
{
	if (m->__type == PTHREAD_MUTEX_RECURSIVE) {
		if (a_fetch_add(&m->__lock, -1)==1 && m->__waiters)
			__wake(&m->__lock, 1, 0);
		return 0;
	}

	if (m->__type == PTHREAD_MUTEX_ERRORCHECK
	 && m->__owner != pthread_self())
	 	return EPERM;

	m->__owner = 0;
	m->__lock = 0;
	if (m->__waiters) __wake(&m->__lock, 1, 0);
	return 0;
}
