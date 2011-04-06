#include <time.h>
#include "pthread_impl.h"

int timer_getoverrun(timer_t t)
{
	if ((uintptr_t)t >= 0x100000) t = ((pthread_t)t)->result;
	return syscall(SYS_timer_getoverrun, (long)t);
}
