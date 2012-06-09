#include "pthread_impl.h"

/* pthread_key_create.c overrides this */
static const size_t dummy = 0;
weak_alias(dummy, __pthread_tsd_size);

int pthread_attr_setstack(pthread_attr_t *a, void *addr, size_t size)
{
	if (size-PTHREAD_STACK_MIN-__pthread_tsd_size > SIZE_MAX/4)
		return EINVAL;
	a->_a_stackaddr = (size_t)addr + size;
	a->_a_stacksize = size - DEFAULT_STACK_SIZE;
	return 0;
}
