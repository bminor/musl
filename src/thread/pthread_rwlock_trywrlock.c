#include "pthread_impl.h"

int pthread_rwlock_trywrlock(pthread_rwlock_t *rw)
{
	if (a_xchg(&rw->__wrlock, 1))
		return EAGAIN;
	if (rw->__readers) {
		a_store(&rw->__wrlock, 0);
		return EAGAIN;
	}
	rw->__owner = pthread_self()->tid;
	return 0;
}
