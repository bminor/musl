#include "pthread_impl.h"

int pthread_attr_setschedparam(pthread_attr_t *a, const struct sched_param *param)
{
	if (param->sched_priority) return ENOTSUP;
	return 0;
}
