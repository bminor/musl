#include "pthread_impl.h"

int pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
	while (pthread_rwlock_tryrdlock(rw))
		__wait(&rw->_rw_wrlock, &rw->_rw_waiters, 1, 0);
	return 0;
}
