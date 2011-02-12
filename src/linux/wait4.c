#include <sys/wait.h>
#include <sys/resource.h>
#include <string.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

pid_t wait4(pid_t pid, int *status, int options, struct rusage *usage)
{
	pid_t ret = syscall4(__NR_wait4, pid, (long)status, options, (long)usage);
	/* Fixup kernel time_t... */
	if (usage) {
		long kusage[4];
		memcpy(kusage, usage, sizeof kusage);
		memmove((struct timeval *)usage + 2, (long *)usage + 4, sizeof *usage - 2*sizeof(struct timeval));
		usage->ru_utime = (struct timeval){ kusage[0], kusage[1] };
		usage->ru_stime = (struct timeval){ kusage[2], kusage[3] };
	}
	return ret;
}
