#include <signal.h>
#include <errno.h>
#include "syscall.h"

int __sigprocmask(int, const sigset_t *, sigset_t *);

int raise(int sig)
{
	int pid, tid, ret;
	sigset_t set;
	sigfillset(&set);
	__sigprocmask(SIG_BLOCK, &set, &set);
	tid = syscall(SYS_gettid);
	pid = syscall(SYS_getpid);
	ret = syscall(SYS_tgkill, pid, tid, sig);
	__sigprocmask(SIG_SETMASK, &set, 0);
	return ret;
}
