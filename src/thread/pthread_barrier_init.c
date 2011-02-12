#include "pthread_impl.h"

int pthread_barrier_init(pthread_barrier_t *b, const pthread_barrierattr_t *a, unsigned count)
{
	if (!count) return EINVAL;
	*b = (pthread_barrier_t){ .__limit = count-1 };
	return 0;
}
