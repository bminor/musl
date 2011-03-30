#include <time.h>
#include "pthread_impl.h"

int timer_settime(timer_t t, int flags, const struct itimerspec *val, struct itimerspec *old)
{
	if ((uintptr_t)t & 1) t = (void *)((unsigned long)t / 2);
	else t = ((pthread_t)t)->result;
	return syscall(SYS_timer_settime, (long)t, flags, val, old);
}
