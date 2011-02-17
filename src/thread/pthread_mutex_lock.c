#include "pthread_impl.h"

int pthread_mutex_lock(pthread_mutex_t *m)
{
	int r;
	while ((r=pthread_mutex_trylock(m)) == EBUSY)
		__wait(&m->_m_lock, &m->_m_waiters, 1, 0);
	return r;
}
