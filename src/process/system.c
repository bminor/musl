#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include "pthread_impl.h"
#include "libc.h"

static void dummy_0()
{
}
weak_alias(dummy_0, __acquire_ptc);
weak_alias(dummy_0, __release_ptc);

pid_t __vfork(void);
void __testcancel(void);

int system(const char *cmd)
{
	pid_t pid;
	sigset_t old;
	struct sigaction sa = { .sa_handler = SIG_IGN }, oldint, oldquit;
	int status = -1, i;

	__testcancel();

	if (!cmd) return 1;

	sigaction(SIGINT, &sa, &oldint);
	sigaction(SIGQUIT, &sa, &oldquit);
	sigprocmask(SIG_BLOCK, SIGALL_SET, &old);

	__acquire_ptc();
	pid = __vfork();

	if (pid) __release_ptc();

	if (pid > 0) {
		sigset_t new = old;
		sigaddset(&new, SIGCHLD);
		sigprocmask(SIG_BLOCK, &new, 0);
		while (waitpid(pid, &status, 0) && errno == EINTR);
	}

	if (pid) {
		sigaction(SIGINT, &oldint, NULL);
		sigaction(SIGQUIT, &oldquit, NULL);
		sigprocmask(SIG_SETMASK, &old, NULL);
		return status;
	}

	/* Before we can unblock signals in the child, all signal
	 * handlers must be eliminated -- even implementation-internal
	 * ones. Otherwise, a signal handler could run in the child
	 * and clobber the parent's memory (due to vfork). */
	for (i=1; i<=8*__SYSCALL_SSLEN; i++) {
		struct sigaction sa;
		__libc_sigaction(i, 0, &sa);
		if (sa.sa_handler!=SIG_IGN && sa.sa_handler!=SIG_DFL) {
			sa.sa_handler = SIG_DFL;
			__libc_sigaction(i, &sa, 0);
		}
	}

	sigprocmask(SIG_SETMASK, &old, NULL);
	execl("/bin/sh", "sh", "-c", cmd, (char *)0);
	_exit(127);
}
