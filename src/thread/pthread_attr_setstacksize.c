#include "pthread_impl.h"

int pthread_attr_setstacksize(pthread_attr_t *a, size_t size)
{
	if (size-PAGE_SIZE > SIZE_MAX/4) return EINVAL;
	a->__stacksize = size - DEFAULT_STACK_SIZE;
	return 0;
}
