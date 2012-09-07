#include "pthread_impl.h"

int pthread_attr_getschedparam(const pthread_attr_t *restrict a, struct sched_param *restrict param)
{
	param->sched_priority = 0;
	return 0;
}
