#include <semaphore.h>
#include "pthread_impl.h"

int sem_trywait(sem_t *sem)
{
	if (a_fetch_add(sem->__val, -1) > 0) return 0;
	if (!a_fetch_add(sem->__val, 1) && sem->__val[1])
		__wake(sem->__val, 1, 0);
	errno = EAGAIN;
	return -1;
}
