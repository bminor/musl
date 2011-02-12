#include "pthread_impl.h"

int pthread_setspecific(pthread_key_t k, const void *x)
{
	struct pthread *self = pthread_self();
	/* Handle the case of the main thread */
	if (!self->tsd) {
		if (!x) return 0;
		if (!(self->tsd = calloc(sizeof(void *), PTHREAD_KEYS_MAX)))
			return ENOMEM;
	}
	/* Avoid unnecessary COW */
	if (self->tsd[k] != x) {
		self->tsd[k] = (void *)x;
		self->tsd_used = 1;
	}
	return 0;
}
