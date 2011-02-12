#include <sys/resource.h>
#include <string.h>
#include "syscall.h"

/* this is a huge hack to make up for the kernel's stupid 32bit time_t
 * without having to recopy the whole rusage structure ourselves.. */

int getrusage(int who, struct rusage *ru)
{
	struct { long tv_sec, tv_usec; } ktv[2];
	char *fakeaddr = ((char *)ru + sizeof(struct timeval [2]) - sizeof ktv);
	if (syscall2(__NR_getrusage, who, (long)fakeaddr) < 0)
		return -1;
	memcpy(ktv, fakeaddr, sizeof ktv);
	ru->ru_utime.tv_sec  = ktv[0].tv_sec;
	ru->ru_utime.tv_usec = ktv[0].tv_usec;
	ru->ru_stime.tv_sec  = ktv[1].tv_sec;
	ru->ru_stime.tv_usec = ktv[1].tv_usec;
	return 0;
}
