#include "pthread_impl.h"

void *pthread_getspecific(pthread_key_t k)
{
	struct pthread *self = pthread_self();
	if (!self->tsd) return 0;
	return self->tsd[k];
}
