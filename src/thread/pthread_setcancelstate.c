#include "pthread_impl.h"

int pthread_setcancelstate(int new, int *old)
{
	struct pthread *self = pthread_self();
	if (old) *old = self->canceldisable & 1;
	if ((unsigned)new > 1) return EINVAL;
	self->canceldisable = (self->canceldisable & ~1) | new;
	return 0;
}
