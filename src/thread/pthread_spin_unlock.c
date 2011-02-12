#include "pthread_impl.h"

int pthread_spin_unlock(pthread_spinlock_t *s)
{
	return *s = 0;
}
