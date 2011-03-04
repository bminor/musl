#include <semaphore.h>
#include "pthread_impl.h"

int sem_trywait(sem_t *sem)
{
	int val = a_fetch_add(sem->__val, -1);
	if (val > 0) return 0;
	a_inc(sem->__val);
	errno = EAGAIN;
	return -1;
}
