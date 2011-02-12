#include <sys/times.h>
#include "syscall.h"

clock_t times(struct tms *tms)
{
	return syscall1(__NR_times, (long)&tms);
}
