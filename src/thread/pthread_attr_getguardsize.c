#include "pthread_impl.h"

int pthread_attr_getguardsize(pthread_attr_t *a, size_t *size)
{
	*size = a->__guardsize + DEFAULT_GUARD_SIZE;
	return 0;
}
