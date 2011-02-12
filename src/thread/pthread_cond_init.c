#include "pthread_impl.h"

int pthread_cond_init(pthread_cond_t *c, const pthread_condattr_t *a)
{
	memset(c, 0, sizeof *c);
	return 0;
}
