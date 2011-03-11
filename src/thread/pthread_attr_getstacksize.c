#include "pthread_impl.h"

int pthread_attr_getstacksize(const pthread_attr_t *a, size_t *size)
{
	*size = a->_a_stacksize + DEFAULT_STACK_SIZE;
	return 0;
}
