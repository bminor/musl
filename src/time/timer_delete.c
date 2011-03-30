#include <time.h>
#include "pthread_impl.h"

int timer_delete(timer_t t)
{
	pthread_t td = 0;
	int r;
	if ((uintptr_t)t & 1) t = (void *)((unsigned long)t / 2);
	else td = t, t = td->result;
	r = __syscall(SYS_timer_delete, (long)t);
	if (td) pthread_cancel(td);
	return r;
}
