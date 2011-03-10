#include <signal.h>
#include <errno.h>
#include "syscall.h"

int raise(int sig)
{
	int pid, tid, ret;
	/* Getting the pid/tid pair is not atomic, and could give wrong
	 * result if a fork occurs in a signal handler between the two
	 * syscalls. Use the tgkill syscall's ESRCH semantics to detect
	 * this condition and retry. */
	do {
		tid = syscall0(__NR_gettid);
		pid = syscall0(__NR_getpid);
		ret = syscall3(__NR_tgkill, pid, tid, sig);
	} while (ret<0 && errno == ESRCH);
	return ret;
}
