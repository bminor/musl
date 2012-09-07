#include "pthread_impl.h"

int pthread_mutex_init(pthread_mutex_t *restrict m, const pthread_mutexattr_t *restrict a)
{
	memset(m, 0, sizeof *m);
	if (a) m->_m_type = *a & 7;
	return 0;
}
