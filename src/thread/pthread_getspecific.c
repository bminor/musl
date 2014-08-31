#include "pthread_impl.h"

static void *__pthread_getspecific(pthread_key_t k)
{
	struct pthread *self = __pthread_self();
	return self->tsd[k];
}

weak_alias(__pthread_getspecific, pthread_getspecific);
