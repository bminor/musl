#include "pthread_impl.h"

int pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
	while (pthread_rwlock_tryrdlock(rw))
		__wait(&rw->__wrlock, &rw->__waiters, 1, 0);
	return 0;
}
