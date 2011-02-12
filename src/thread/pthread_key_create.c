#include "pthread_impl.h"

const size_t __pthread_tsd_size = sizeof(void *) * PTHREAD_KEYS_MAX;

static void nodtor(void *dummy)
{
}

int pthread_key_create(pthread_key_t *k, void (*dtor)(void *))
{
	static void (*keys[PTHREAD_KEYS_MAX])(void *);
	int i = (uintptr_t)&k / 16 % PTHREAD_KEYS_MAX;
	int j = i;

	libc.tsd_keys = keys;
	if (!dtor) dtor = nodtor;
	/* Cheap trick - &k cannot match any destructor pointer */
	while (a_cas_p(keys+j, 0, &k)
		&& (j=(j+1)%PTHREAD_KEYS_MAX) != i);
	if (keys[j] != (void (*)(void *))&k)
		return EAGAIN;
	keys[j] = dtor;
	*k = j;
	return 0;
}
