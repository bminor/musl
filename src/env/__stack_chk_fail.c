#include <string.h>
#include <stdint.h>
#include "pthread_impl.h"
#include "atomic.h"

uintptr_t __stack_chk_guard;

void __init_ssp(void *entropy)
{
	/* Here the thread pointer is used without checking whether
	 * it is available; this will crash if it's not. However,
	 * this function is only meant to be called if the program
	 * being run uses stack protector, and in that case, it would
	 * crash without a thread pointer anyway, so it's better to
	 * crash early before there is state to be lost on crash. */
	pthread_t self = __pthread_self();
	uintptr_t canary;
	if (entropy) memcpy(&canary, entropy, sizeof canary);
	else canary = (uintptr_t)&canary * 1103515245;
	a_cas_l(&__stack_chk_guard, 0, canary);
	self->canary = __stack_chk_guard;
}

void __stack_chk_fail(void)
{
	a_crash();
}
