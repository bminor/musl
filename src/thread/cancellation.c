#include "pthread_impl.h"

static void dummy(struct __ptcb *cb)
{
}
weak_alias(dummy, __pthread_do_unwind);
weak_alias(dummy, __pthread_do_register);
weak_alias(dummy, __pthread_do_unregister);

void __pthread_unwind_next(struct __ptcb *cb)
{
	__pthread_do_unwind(cb);
}

void __pthread_register_cancel(struct __ptcb *cb)
{
	__pthread_do_register(cb);
}

void __pthread_unregister_cancel(struct __ptcb *cb)
{
	__pthread_do_unregister(cb);
}
