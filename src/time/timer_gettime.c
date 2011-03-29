#include <time.h>
#include "pthread_impl.h"

int timer_gettime(timer_t t, struct itimerspec *val)
{
	return syscall(SYS_timer_gettime, t->timerid, val);
}
