#include "pthread_impl.h"

int pthread_cond_signal(pthread_cond_t *c)
{
	int w = c->_c_waiters;
	if (a_swap(&c->_c_block, 0) || w)
		__wake(&c->_c_block, 1, 0);
	return 0;
}
