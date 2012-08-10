#include <signal.h>
#include <errno.h>
#include <stdint.h>
#include "syscall.h"
#include "pthread_impl.h"

int raise(int sig)
{
	int pid, tid, ret;
	sigset_t set;
	__syscall(SYS_rt_sigprocmask, SIG_BLOCK, SIGALL_SET, &set, __SYSCALL_SSLEN);
	tid = syscall(SYS_gettid);
	pid = syscall(SYS_getpid);
	ret = syscall(SYS_tgkill, pid, tid, sig);
	__syscall(SYS_rt_sigprocmask, SIG_SETMASK, &set, 0, __SYSCALL_SSLEN);
	return ret;
}
