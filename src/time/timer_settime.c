#include <time.h>
#include "pthread_impl.h"

int timer_settime(timer_t t, int flags, const struct itimerspec *val, struct itimerspec *old)
{
	return syscall(SYS_timer_settime, t->timerid, flags, val, old);
}
