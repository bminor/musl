#include "pthread_impl.h"

#undef pthread_self

void pthread_exit(void *result)
{
	int i;
	struct pthread *self = pthread_self();
	self->result = result;

	a_dec(&libc.threads_minus_1);
	if (libc.threads_minus_1 < 0)
		exit(0);

	LOCK(&self->exitlock);

	if (self->tsd_used) for (i=0; i<PTHREAD_KEYS_MAX; i++)
		if (self->tsd[i] && libc.tsd_keys[i])
			libc.tsd_keys[i](self->tsd[i]);

	if (self->detached && self->map_base)
		__unmapself(self->map_base, self->map_size);

	__syscall_exit(0);
}
