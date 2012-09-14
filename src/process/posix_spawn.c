#include <spawn.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <fcntl.h>
#include "syscall.h"
#include "fdop.h"

extern char **environ;

pid_t __vfork(void);

int __posix_spawnx(pid_t *restrict res, const char *restrict path,
	int (*exec)(const char *, char *const *),
	const posix_spawn_file_actions_t *fa,
	const posix_spawnattr_t *restrict attr,
	char *const argv[restrict], char *const envp[restrict])
{
	pid_t pid;
	sigset_t oldmask;
	int i;
	posix_spawnattr_t dummy_attr = { 0 };

	if (!attr) attr = &dummy_attr;

	sigprocmask(SIG_BLOCK, (void *)(uint64_t []){-1}, &oldmask);
	pid = __vfork();

	if (pid) {
		sigprocmask(SIG_SETMASK, &oldmask, 0);
		if (pid < 0) return -pid;
		*res = pid;
		return 0;
	}

	for (i=1; i<=64; i++) {
		struct sigaction sa;
		sigaction(i, 0, &sa);
		if (sa.sa_handler!=SIG_IGN ||
		    ((attr->__flags & POSIX_SPAWN_SETSIGDEF)
		     && sigismember(&attr->__def, i) )) {
			sa.sa_handler = SIG_DFL;
			sigaction(i, &sa, 0);
		}
	}

	if ((attr->__flags&POSIX_SPAWN_SETPGROUP) && setpgid(0, attr->__pgrp))
		_exit(127);

	/* Use syscalls directly because pthread state is not consistent
	 * for making calls to the library wrappers... */
	if ((attr->__flags&POSIX_SPAWN_RESETIDS) && (
		__syscall(SYS_setgid, __syscall(SYS_getgid)) ||
		__syscall(SYS_setuid, __syscall(SYS_getuid)) ))
		_exit(127);

	if (fa && fa->__actions) {
		struct fdop *op;
		int ret, fd;
		for (op = fa->__actions; op->next; op = op->next);
		for (; op; op = op->prev) {
			switch(op->cmd) {
			case FDOP_CLOSE:
				ret = __syscall(SYS_close, op->fd);
				break;
			case FDOP_DUP2:
				ret = __syscall(SYS_dup2, op->fd, op->newfd)<0;
				break;
			case FDOP_OPEN:
				fd = __syscall(SYS_open, op->path,
					op->oflag | O_LARGEFILE, op->mode);
				if (fd == op->fd) {
					ret = 0;
				} else {
					ret = __syscall(SYS_dup2, fd, op->fd)<0;
					__syscall(SYS_close, fd);
				}
				break;
			}
			if (ret) _exit(127);
		}
	}

	sigprocmask(SIG_SETMASK, (attr->__flags & POSIX_SPAWN_SETSIGMASK)
		? &attr->__mask : &oldmask, 0);

	if (envp) environ = (char **)envp;
	exec(path, argv);
	_exit(127);

	return 0;
}

int posix_spawn(pid_t *restrict res, const char *restrict path,
	const posix_spawn_file_actions_t *fa,
	const posix_spawnattr_t *restrict attr,
	char *const argv[restrict], char *const envp[restrict])
{
	return __posix_spawnx(res, path, execv, fa, attr, argv, envp);
}
