#include <time.h>
#include "pthread_impl.h"

int timer_settime(timer_t t, int flags, const struct itimerspec *restrict val, struct itimerspec *restrict old)
{
	if ((uintptr_t)t >= 0x100000) t = ((pthread_t)t)->result;
	return syscall(SYS_timer_settime, (long)t, flags, val, old);
}
