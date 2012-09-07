#include "pthread_impl.h"

int pthread_condattr_getclock(const pthread_condattr_t *restrict a, clockid_t *restrict clk)
{
	*clk = *a & 0x7fffffff;
	return 0;
}
