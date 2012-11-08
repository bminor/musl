#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "stdio_impl.h"
#include "pthread_impl.h"
#include "syscall.h"

static void dummy_0()
{
}
weak_alias(dummy_0, __acquire_ptc);
weak_alias(dummy_0, __release_ptc);

pid_t __vfork(void);

FILE *popen(const char *cmd, const char *mode)
{
	int p[2], op, i;
	pid_t pid;
	FILE *f;
	sigset_t old;
	const char *modes = "rw", *mi = strchr(modes, *mode);

	if (mi) {
		op = mi-modes;
	} else {
		errno = EINVAL;
		return 0;
	}
	
	if (pipe2(p, O_CLOEXEC)) return NULL;
	f = fdopen(p[op], mode);
	if (!f) {
		__syscall(SYS_close, p[0]);
		__syscall(SYS_close, p[1]);
		return NULL;
	}

	sigprocmask(SIG_BLOCK, SIGALL_SET, &old);
	
	__acquire_ptc();
	pid = __vfork();

	if (pid) {
		__release_ptc();
		__syscall(SYS_close, p[1-op]);
		sigprocmask(SIG_SETMASK, &old, 0);
		if (pid < 0) {
			fclose(f);
			return 0;
		}
		f->pipe_pid = pid;
		return f;
	}

	/* See notes in system.c for why this is needed. */
	for (i=1; i<=8*__SYSCALL_SSLEN; i++) {
		struct sigaction sa;
		__libc_sigaction(i, 0, &sa);
		if (sa.sa_handler!=SIG_IGN && sa.sa_handler!=SIG_DFL) {
			sa.sa_handler = SIG_DFL;
			__libc_sigaction(i, &sa, 0);
		}
	}
	if (dup2(p[1-op], 1-op) < 0) _exit(127);
	fcntl(1-op, F_SETFD, 0);
	if (p[0] != 1-op) __syscall(SYS_close, p[0]);
	if (p[1] != 1-op) __syscall(SYS_close, p[1]);
	sigprocmask(SIG_SETMASK, &old, 0);
	execl("/bin/sh", "sh", "-c", cmd, (char *)0);
	_exit(127);
}
