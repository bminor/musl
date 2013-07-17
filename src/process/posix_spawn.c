#define _GNU_SOURCE
#include <spawn.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "syscall.h"
#include "pthread_impl.h"
#include "fdop.h"
#include "libc.h"

struct args {
	int p[2];
	sigset_t oldmask;
	const char *path;
	int (*exec)(const char *, char *const *, char *const *);
	const posix_spawn_file_actions_t *fa;
	const posix_spawnattr_t *restrict attr;
	char *const *argv, *const *envp;
};

static int child(void *args_vp)
{
	int i, ret;
	struct sigaction sa;
	struct args *args = args_vp;
	int p = args->p[1];
	const posix_spawn_file_actions_t *fa = args->fa;
	const posix_spawnattr_t *restrict attr = args->attr;

	close(args->p[0]);

	/* All signal dispositions must be either SIG_DFL or SIG_IGN
	 * before signals are unblocked. Otherwise a signal handler
	 * from the parent might get run in the child while sharing
	 * memory, with unpredictable and dangerous results. */
	for (i=1; i<_NSIG; i++) {
		__libc_sigaction(i, 0, &sa);
		if (sa.sa_handler!=SIG_DFL && (sa.sa_handler!=SIG_IGN ||
		    ((attr->__flags & POSIX_SPAWN_SETSIGDEF)
		     && sigismember(&attr->__def, i) ))) {
			sa.sa_handler = SIG_DFL;
			__libc_sigaction(i, &sa, 0);
		}
	}

	if (attr->__flags & POSIX_SPAWN_SETPGROUP)
		if ((ret=__syscall(SYS_setpgid, 0, attr->__pgrp)))
			goto fail;

	/* Use syscalls directly because pthread state because the
	 * library functions attempt to do a multi-threaded synchronized
	 * id-change, which would trash the parent's state. */
	if (attr->__flags & POSIX_SPAWN_RESETIDS)
		if ((ret=__syscall(SYS_setgid, __syscall(SYS_getgid))) ||
		    (ret=__syscall(SYS_setuid, __syscall(SYS_getuid))) )
			goto fail;

	if (fa && fa->__actions) {
		struct fdop *op;
		int fd;
		for (op = fa->__actions; op->next; op = op->next);
		for (; op; op = op->prev) {
			/* It's possible that a file operation would clobber
			 * the pipe fd used for synchronizing with the
			 * parent. To avoid that, we dup the pipe onto
			 * an unoccupied fd. */
			if (op->fd == p) {
				ret = __syscall(SYS_dup, p);
				if (ret < 0) goto fail;
				__syscall(SYS_close, p);
				p = ret;
			}
			switch(op->cmd) {
			case FDOP_CLOSE:
				if ((ret=__syscall(SYS_close, op->fd)))
					goto fail;
				break;
			case FDOP_DUP2:
				if ((ret=__syscall(SYS_dup2, op->srcfd, op->fd))<0)
					goto fail;
				break;
			case FDOP_OPEN:
				fd = __syscall(SYS_open, op->path,
					op->oflag | O_LARGEFILE, op->mode);
				if ((ret=fd) < 0) goto fail;
				if (fd != op->fd) {
					if ((ret=__syscall(SYS_dup2, fd, op->fd))<0)
						goto fail;
					__syscall(SYS_close, fd);
				}
				break;
			}
		}
	}

	/* Close-on-exec flag may have been lost if we moved the pipe
	 * to a different fd. We don't use F_DUPFD_CLOEXEC above because
	 * it would fail on older kernels and atomicity is not needed --
	 * in this process there are no threads or signal handlers. */
	__syscall(SYS_fcntl, p, F_SETFD, FD_CLOEXEC);

	pthread_sigmask(SIG_SETMASK, (attr->__flags & POSIX_SPAWN_SETSIGMASK)
		? &attr->__mask : &args->oldmask, 0);

	args->exec(args->path, args->argv, args->envp);

fail:
	/* Since sizeof errno < PIPE_BUF, the write is atomic. */
	ret = -ret;
	if (ret) while (write(p, &ret, sizeof ret) < 0);
	_exit(127);
}


int __posix_spawnx(pid_t *restrict res, const char *restrict path,
	int (*exec)(const char *, char *const *, char *const *),
	const posix_spawn_file_actions_t *fa,
	const posix_spawnattr_t *restrict attr,
	char *const argv[restrict], char *const envp[restrict])
{
	pid_t pid;
	char stack[1024];
	int ec=0, cs;
	struct args args;

	if (pipe2(args.p, O_CLOEXEC))
		return errno;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	args.path = path;
	args.exec = exec;
	args.fa = fa;
	args.attr = attr ? attr : &(const posix_spawnattr_t){0};
	args.argv = argv;
	args.envp = envp;
	pthread_sigmask(SIG_BLOCK, SIGALL_SET, &args.oldmask);

	pid = __clone(child, stack+sizeof stack,
		CLONE_VM|CLONE_VFORK|SIGCHLD, &args);
	close(args.p[1]);

	if (pid > 0) {
		if (read(args.p[0], &ec, sizeof ec) != sizeof ec) ec = 0;
		else waitpid(pid, &(int){0}, 0);
	} else {
		ec = -pid;
	}

	close(args.p[0]);

	if (!ec) *res = pid;

	pthread_sigmask(SIG_SETMASK, &args.oldmask, 0);
	pthread_setcancelstate(cs, 0);

	return ec;
}

int posix_spawn(pid_t *restrict res, const char *restrict path,
	const posix_spawn_file_actions_t *fa,
	const posix_spawnattr_t *restrict attr,
	char *const argv[restrict], char *const envp[restrict])
{
	return __posix_spawnx(res, path, execve, fa, attr, argv, envp);
}
