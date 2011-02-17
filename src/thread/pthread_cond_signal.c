#include "pthread_impl.h"

int pthread_cond_signal(pthread_cond_t *c)
{
	c->_c_block = 0;
	__wake(&c->_c_block, 1, 0);
	return 0;
}
