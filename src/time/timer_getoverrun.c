#include <time.h>
#include "pthread_impl.h"

int timer_getoverrun(timer_t t)
{
	return syscall(SYS_timer_getoverrun, t->timerid);
}
