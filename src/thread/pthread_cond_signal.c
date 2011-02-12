#include "pthread_impl.h"

int pthread_cond_signal(pthread_cond_t *c)
{
	c->__block = 0;
	__wake(&c->__block, 1, 0);
	return 0;
}
