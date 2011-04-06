#include <time.h>
#include "pthread_impl.h"

int timer_delete(timer_t t)
{
	if ((uintptr_t)t >= 0x100000) return pthread_cancel(t);
	return __syscall(SYS_timer_delete, (long)t);
}
