#include "pthread_impl.h"

void __pthread_register_cancel(struct __ptcb *cb)
{
	struct pthread *self = pthread_self();
	cb->__next = self->cancelbuf;
	self->cancelbuf = cb;
}

#define pthread_self __pthread_self

void __pthread_unregister_cancel(struct __ptcb *cb)
{
	struct pthread *self = pthread_self();
	self->cancelbuf = self->cancelbuf->__next;
}

void __pthread_unwind_next(struct __ptcb *cb)
{
	if (cb->__next) longjmp((void *)cb->__next->__jb, 1);
	pthread_exit(PTHREAD_CANCELLED);
}
