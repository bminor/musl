#include <string.h>
#include <inttypes.h>
#include <elf.h>
#include "pthread_impl.h"
#include "atomic.h"

uintptr_t __stack_chk_guard;

void __init_ssp(size_t *auxv)
{
	size_t i;
	pthread_t self = __pthread_self_init();
	uintptr_t canary;
	for (i=0; auxv[i] && auxv[i]!=AT_RANDOM; i+=2);
	if (auxv[i]) memcpy(&canary, (void *)auxv[i+1], sizeof canary);
	else canary = (uintptr_t)&canary * 1103515245;
	a_cas_l(&__stack_chk_guard, 0, canary);
	self->canary = __stack_chk_guard;
}

void __stack_chk_fail(void)
{
	a_crash();
}
