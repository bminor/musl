#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "syscall.h"

int sigqueue(pid_t pid, int sig, const union sigval value)
{
	siginfo_t si = {
		.si_signo = sig,
		.si_code = -1,
		.si_value = value,
	};
	return syscall3(__NR_rt_sigqueueinfo, pid, sig, (long)&si);
}
