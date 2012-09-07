#include "pthread_impl.h"

int pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict a, int *restrict robust)
{
	*robust = *a / 4U % 2;
	return 0;
}
