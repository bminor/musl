#include "pthread_impl.h"

int pthread_condattr_getclock(const pthread_condattr_t *a, clockid_t *clk)
{
	*clk = *a & 0x7fffffff;
	return 0;
}
