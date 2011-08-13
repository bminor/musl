#include <time.h>
#include <sys/times.h>
#include "syscall.h"

int __clock_gettime(clockid_t, struct timespec *);

clock_t clock()
{
	struct timespec ts;
	struct tms tms;
	if (!__clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts))
		return ts.tv_sec*1000000 + ts.tv_nsec/1000;
	__syscall(SYS_times, &tms);
	return (tms.tms_utime + tms.tms_stime)*100;
}
