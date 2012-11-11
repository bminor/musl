#include "pthread_impl.h"

int pthread_attr_getinheritsched(const pthread_attr_t *a, int *inherit)
{
	*inherit = a->_a_sched;
	return 0;
}
