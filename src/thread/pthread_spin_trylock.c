#include "pthread_impl.h"

int pthread_spin_trylock(pthread_spinlock_t *s)
{
	return -a_xchg(s, 1) & EBUSY;
}
