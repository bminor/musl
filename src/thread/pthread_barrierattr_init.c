#include "pthread_impl.h"

int pthread_barrierattr_init(pthread_barrierattr_t *a)
{
	memset(a, 0, sizeof *a);
	return 0;
}
