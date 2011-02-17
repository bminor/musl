#include "pthread_impl.h"

int pthread_rwlock_wrlock(pthread_rwlock_t *rw)
{
	int nr;
	while (pthread_rwlock_trywrlock(rw)==EAGAIN) {
		if ((nr=rw->_rw_readers))
			__wait(&rw->_rw_readers, &rw->_rw_waiters, nr, 0);
		else
			__wait(&rw->_rw_wrlock, &rw->_rw_waiters, 1, 0);
	}
	return 0;
}
