#include "pthread_impl.h"

int pthread_mutexattr_init(pthread_mutexattr_t *a)
{
	memset(a, 0, sizeof *a);
	return 0;
}
