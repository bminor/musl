#include <time.h>
#include "pthread_impl.h"

int timer_delete(timer_t t)
{
	if (t->thread) pthread_cancel(t->thread);
	else {
		__syscall(SYS_timer_delete, t->timerid);
		free(t);
	}
	return 0;
}
