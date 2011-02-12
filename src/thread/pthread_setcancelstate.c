#include "pthread_impl.h"

int pthread_setcancelstate(int new, int *old)
{
	struct pthread *self = pthread_self();
	if (old) *old = self->canceldisable;
	if ((unsigned)new > 1) return EINVAL;
	self->canceldisable = new;
	return 0;
}
