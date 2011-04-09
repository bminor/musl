#include <time.h>
#include "pthread_impl.h"

int timer_delete(timer_t t)
{
	if ((uintptr_t)t >= 0x100000) {
		pthread_t td = t;
		td->delete_timer = 1;
		__wake(&td->delete_timer, 1, 1);
		return 0;
	}
	return __syscall(SYS_timer_delete, (long)t);
}
