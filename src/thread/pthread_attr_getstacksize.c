#include "pthread_impl.h"

int pthread_attr_getstacksize(const pthread_attr_t *restrict a, size_t *restrict size)
{
	*size = a->_a_stacksize + DEFAULT_STACK_SIZE;
	return 0;
}
