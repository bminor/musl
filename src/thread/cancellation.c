#include "pthread_impl.h"

void __pthread_register_cancel(struct __ptcb *cb)
{
	struct pthread *self = pthread_self();
	cb->__next = self->cancelbuf;
	self->cancelbuf = cb;
}

void __pthread_unregister_cancel(struct __ptcb *cb)
{
	struct pthread *self = __pthread_self();
	self->cancelbuf = self->cancelbuf->__next;
}
