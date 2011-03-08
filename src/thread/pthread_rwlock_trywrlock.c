#include "pthread_impl.h"

int pthread_rwlock_trywrlock(pthread_rwlock_t *rw)
{
	if (a_xchg(&rw->_rw_wrlock, 1))
		return EBUSY;
	if (rw->_rw_readers) {
		a_store(&rw->_rw_wrlock, 0);
		return EBUSY;
	}
	rw->_rw_owner = pthread_self()->tid;
	return 0;
}
