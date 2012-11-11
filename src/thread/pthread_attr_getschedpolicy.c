#include "pthread_impl.h"

int pthread_attr_getschedpolicy(const pthread_attr_t *a, int *policy)
{
	*policy = a->_a_policy;
	return 0;
}
