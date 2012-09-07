#include "pthread_impl.h"

int pthread_attr_getstack(const pthread_attr_t *restrict a, void **restrict addr, size_t *restrict size)
{
	if (!a->_a_stackaddr)
		return EINVAL;
	*size = a->_a_stacksize + DEFAULT_STACK_SIZE;
	*addr = (void *)(a->_a_stackaddr - *size);
	return 0;
}
