#include "pthread_impl.h"

int pthread_attr_init(pthread_attr_t *a)
{
	memset(a, 0, sizeof *a);
	return 0;
}
