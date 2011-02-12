#include "pthread_impl.h"

int pthread_attr_getstacksize(pthread_attr_t *a, size_t *size)
{
	*size = a->__stacksize + DEFAULT_STACK_SIZE;
	return 0;
}
