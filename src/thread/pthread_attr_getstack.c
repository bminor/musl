#include "pthread_impl.h"

int pthread_attr_getstack(const pthread_attr_t *a, void **addr, size_t *size)
{
	if (!a->_a_stackaddr)
		return EINVAL;
	*size = a->_a_stacksize + DEFAULT_STACK_SIZE;
	*addr = (void *)(a->_a_stackaddr - *size);
	return 0;
}
