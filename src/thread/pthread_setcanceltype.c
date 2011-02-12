#include "pthread_impl.h"

int pthread_setcanceltype(int new, int *old)
{
	struct pthread *self = pthread_self();
	if (old) *old = self->cancelasync;
	if ((unsigned)new > 1) return EINVAL;
	self->cancelasync = new;
	return 0;
}
