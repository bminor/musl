#include <signal.h>
#include <errno.h>
#include <stdint.h>
#include "syscall.h"
#include "pthread_impl.h"

int raise(int sig)
{
	int pid, tid, ret;
	sigset_t set;
	__syscall(SYS_rt_sigprocmask, SIG_BLOCK, SIGALL_SET, &set, _NSIG/8);
	tid = syscall(SYS_gettid);
	pid = syscall(SYS_getpid);
	ret = syscall(SYS_tgkill, pid, tid, sig);
	__syscall(SYS_rt_sigprocmask, SIG_SETMASK, &set, 0, _NSIG/8);
	return ret;
}
