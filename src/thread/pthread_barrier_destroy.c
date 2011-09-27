#include "pthread_impl.h"

void __vm_lock(int), __vm_unlock(void);

int pthread_barrier_destroy(pthread_barrier_t *b)
{
	if (b->_b_limit < 0) {
		__vm_lock(-1);
		__vm_unlock();
	}
	return 0;
}
