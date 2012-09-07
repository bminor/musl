#include "pthread_impl.h"

int pthread_attr_getguardsize(const pthread_attr_t *restrict a, size_t *restrict size)
{
	*size = a->_a_guardsize + DEFAULT_GUARD_SIZE;
	return 0;
}
