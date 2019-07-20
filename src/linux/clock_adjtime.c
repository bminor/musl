#include <sys/timex.h>
#include <time.h>
#include "syscall.h"

int clock_adjtime (clockid_t clock_id, struct timex *utx)
{
	if (clock_id==CLOCK_REALTIME) return syscall(SYS_adjtimex, utx);
	return syscall(SYS_clock_adjtime, clock_id, utx);
}
