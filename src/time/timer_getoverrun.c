#include <time.h>
#include "pthread_impl.h"

int timer_getoverrun(timer_t t)
{
	if ((uintptr_t)t & 1) t = (void *)((unsigned long)t / 2);
	else t = ((pthread_t)t)->result;
	return syscall(SYS_timer_getoverrun, (long)t);
}
