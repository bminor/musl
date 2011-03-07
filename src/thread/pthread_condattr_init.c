#include "pthread_impl.h"

int pthread_condattr_init(pthread_condattr_t *a)
{
	memset(a, 0, sizeof *a);
	return 0;
}
