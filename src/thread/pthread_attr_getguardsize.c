#include "pthread_impl.h"

int pthread_attr_getguardsize(pthread_attr_t *a, size_t *size)
{
	*size = a->_a_guardsize + DEFAULT_GUARD_SIZE;
	return 0;
}
