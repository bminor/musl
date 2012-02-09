#include "pthread_impl.h"

void __do_cleanup_push();
void __do_cleanup_pop();

void _pthread_cleanup_push(struct __ptcb *cb, void (*f)(void *), void *x)
{
	__do_cleanup_push(cb, f, x);
}

void _pthread_cleanup_pop(struct __ptcb *cb, int run)
{
	__do_cleanup_pop(cb, run);
}

static void dummy()
{
}
weak_alias(dummy, __do_cleanup_push);
weak_alias(dummy, __do_cleanup_pop);
