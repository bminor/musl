#include <semaphore.h>
#include "pthread_impl.h"

int sem_trywait(sem_t *sem)
{
	int val = sem->__val[0];
	if (val>0 && a_cas(sem->__val, val, val-1)==val) return 0;
	errno = EAGAIN;
	return -1;
}
