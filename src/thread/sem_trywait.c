#include <semaphore.h>
#include "pthread_impl.h"

int sem_trywait(sem_t *sem)
{
	int val = sem->__val[0];
	if (val>0) {
		int new = val-1-(val==1 && sem->__val[1]);
		if (a_cas(sem->__val, val, new)==val) return 0;
	}
	errno = EAGAIN;
	return -1;
}
