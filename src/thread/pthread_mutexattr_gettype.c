#include "pthread_impl.h"

int pthread_mutexattr_gettype(const pthread_mutexattr_t *a, int *type)
{
	*type = *a & 3;
	return 0;
}
