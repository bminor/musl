#include "pthread_impl.h"

int pthread_cancel(pthread_t t)
{
	t->cancel = 1;
	return pthread_kill(t, SIGCANCEL);
}
