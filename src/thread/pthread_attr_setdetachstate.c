#include "pthread_impl.h"

int pthread_attr_setdetachstate(pthread_attr_t *a, int state)
{
	a->__detach = state;
	return 0;
}
