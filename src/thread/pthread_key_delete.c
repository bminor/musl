#include "pthread_impl.h"

int pthread_key_delete(pthread_key_t k)
{
	libc.tsd_keys[k] = 0;
	return 0;
}
