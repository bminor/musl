#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "syscall.h"

int sigqueue(pid_t pid, int sig, const union sigval value)
{
	siginfo_t si;
	memset(&si, 0, sizeof si);
	si.si_signo = sig;
	si.si_code = SI_QUEUE;
	si.si_value = value;
	si.si_pid = getpid();
	si.si_uid = getuid();
	return syscall(SYS_rt_sigqueueinfo, pid, sig, &si);
}
