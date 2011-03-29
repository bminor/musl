#include "pthread_impl.h"

int pthread_spin_lock(pthread_spinlock_t *s)
{
	while (a_xchg(s, 1));
	return 0;
}
