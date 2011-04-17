#include "pthread_impl.h"

int pthread_setcanceltype(int new, int *old)
{
	struct pthread *self = pthread_self();
	if (new > 1U) return EINVAL;
	if (old) *old = self->cancelasync;
	self->cancelasync = new;
	return 0;
}
