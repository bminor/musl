#include <spawn.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include "syscall.h"

extern char **environ;

int __posix_spawnx(pid_t *res, const char *path,
	int (*exec)(const char *, char *const *),
	const posix_spawn_file_actions_t *fa,
	const posix_spawnattr_t *attr, char **argv, char **envp)
{
	pid_t pid;
	sigset_t oldmask;
	int i;
	posix_spawnattr_t dummy_attr = { 0 };

	if (!attr) attr = &dummy_attr;

	sigprocmask(SIG_BLOCK, (void *)(uint64_t []){-1}, &oldmask);
	pid = __syscall(SYS_fork);

	if (pid) {
		sigprocmask(SIG_SETMASK, &oldmask, 0);
		if (pid < 0) return -pid;
		*res = pid;
		return 0;
	}

	for (i=1; i<=64; i++) {
		struct sigaction sa;
		sigaction(i, 0, &sa);
		if (sa.sa_handler!=SIG_IGN || sigismember(&attr->__def, i)) {
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

	sigprocmask(SIG_SETMASK, (attr->__flags & POSIX_SPAWN_SETSIGMASK)
		? &attr->__mask : &oldmask, 0);

	if (envp) environ = envp;
	exec(path, argv);
	_exit(127);

	return 0;
}

int posix_spawn(pid_t *res, const char *path,
	const posix_spawn_file_actions_t *fa,
	const posix_spawnattr_t *attr, char **argv, char **envp)
{
	return __posix_spawnx(res, path, execv, fa, attr, argv, envp);
}
