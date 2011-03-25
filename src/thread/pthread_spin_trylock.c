#include "pthread_impl.h"

int pthread_spin_trylock(pthread_spinlock_t *s)
{
	if (*s || a_xchg(s, 1)) return EBUSY;
	return 0;
}
