#include <time.h>
#include "pthread_impl.h"

int timer_delete(timer_t t)
{
	if ((uintptr_t)t & 1)
		return __syscall(SYS_timer_delete, ((unsigned long)t / 2));
	return pthread_cancel(t);
}
