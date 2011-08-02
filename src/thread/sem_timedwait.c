#include <semaphore.h>
#include "pthread_impl.h"

static void cleanup(void *p)
{
	a_dec(p);
}

int sem_timedwait(sem_t *sem, const struct timespec *at)
{
	while (sem_trywait(sem)) {
		int r;
		if (at && at->tv_nsec >= 1000000000UL) {
			errno = EINVAL;
			return -1;
		}
		a_inc(sem->__val+1);
		a_cas(sem->__val, 0, -1);
		pthread_cleanup_push(cleanup, sem->__val+1);
		r = __timedwait_cp(sem->__val, -1, CLOCK_REALTIME, at, 0);
		pthread_cleanup_pop(1);
		if (r) {
			errno = r;
			return -1;
		}
	}
	return 0;
}
