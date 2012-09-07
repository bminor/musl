#include "pthread_impl.h"

int pthread_attr_setschedparam(pthread_attr_t *restrict a, const struct sched_param *restrict param)
{
	if (param->sched_priority) return ENOTSUP;
	return 0;
}
