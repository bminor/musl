#include "pthread_impl.h"

int pthread_cond_broadcast(pthread_cond_t *c)
{
	if (a_swap(&c->_c_block, 0))
		__wake(&c->_c_block, -1, 0);
	return 0;
}
