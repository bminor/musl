#include "pthread_impl.h"

int pthread_cancel(pthread_t t)
{
	return pthread_kill(t, SIGCANCEL);
}
