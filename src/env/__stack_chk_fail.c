#include "pthread_impl.h"
#include "atomic.h"

void __init_ssp(void)
{
#ifndef __PIC__
	__pthread_self_init();
#endif
}

void __stack_chk_fail(void)
{
	a_crash();
}
