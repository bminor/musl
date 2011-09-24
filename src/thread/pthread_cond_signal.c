#include "pthread_impl.h"

int pthread_cond_signal(pthread_cond_t *c)
{
	a_store(&c->_c_block, 0);
	if (c->_c_waiters) __wake(&c->_c_block, 1, 0);
	return 0;
}
