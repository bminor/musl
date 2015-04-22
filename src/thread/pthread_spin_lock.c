#include "pthread_impl.h"

int pthread_spin_lock(pthread_spinlock_t *s)
{
	while (*(volatile int *)s || a_cas(s, 0, 1)) a_spin();
	return 0;
}
