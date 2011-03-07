#include "pthread_impl.h"

int pthread_cond_init(pthread_cond_t *c, const pthread_condattr_t *a)
{
	memset(c, 0, sizeof *c);
	if (a) c->_c_clock = *a & 0x7fffffff;
	return 0;
}
