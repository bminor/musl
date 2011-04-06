#include <time.h>
#include "pthread_impl.h"

int timer_gettime(timer_t t, struct itimerspec *val)
{
	if ((uintptr_t)t >= 0x100000) t = ((pthread_t)t)->result;
	return syscall(SYS_timer_gettime, (long)t, val);
}
