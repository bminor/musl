#include "pthread_impl.h"

int pthread_setcancelstate(int new, int *old)
{
	struct pthread *self = pthread_self();
	if (old) *old = self->canceldisable;
	if (new > 1U) return EINVAL;
	self->canceldisable = new;
	return 0;
}
