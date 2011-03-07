#include "pthread_impl.h"

int pthread_condattr_setclock(pthread_condattr_t *a, clockid_t clk)
{
	if (clk < 0) return EINVAL;
	*a &= 0x80000000;
	*a |= clk;
	return 0;
}
