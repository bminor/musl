#include "pthread_impl.h"

int pthread_attr_setscope(pthread_attr_t *a, int scope)
{
	if (scope > 1U) return EINVAL;
	return 0;
}
