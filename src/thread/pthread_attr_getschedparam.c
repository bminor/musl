#include "pthread_impl.h"

int pthread_attr_getschedparam(const pthread_attr_t *a, struct sched_param *param)
{
	param->sched_priority = 0;
	return 0;
}
