#include "pthread_impl.h"

const size_t __pthread_tsd_size = sizeof(void *) * PTHREAD_KEYS_MAX;

static void nodtor(void *dummy)
{
}

int pthread_key_create(pthread_key_t *k, void (*dtor)(void *))
{
	static void (*keys[PTHREAD_KEYS_MAX])(void *);
	unsigned i = (uintptr_t)&k / 16 % PTHREAD_KEYS_MAX;
	unsigned j = i;

	pthread_self();
	libc.tsd_keys = keys;
	if (!dtor) dtor = nodtor;
	do {
		if (!a_cas_p(keys+j, 0, dtor)) {
			*k = j;
			return 0;
		}
	} while ((j=(j+1)%PTHREAD_KEYS_MAX) != i);
	return EAGAIN;
}
