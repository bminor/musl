#include <semaphore.h>
#include "pthread_impl.h"

int sem_post(sem_t *sem)
{
	a_inc(sem->__val);
	if (sem->__val[1])
		__wake(sem->__val, 1, 0);
	return 0;
}
