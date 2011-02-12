#include "pthread_impl.h"

int pthread_key_delete(pthread_key_t k)
{
	if (libc.tsd_keys) libc.tsd_keys[k] = 0;
	return 0;
}
