#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

int system(const char *cmd)
{
	pid_t pid;
	sigset_t old, new;
	struct sigaction sa, oldint, oldquit;
	int status;

	if (!cmd) return 1;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, &oldint);
	sigaction(SIGQUIT, &sa, &oldquit);
	sigaddset(&sa.sa_mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &new, &old);

	pid = fork();
	if (pid <= 0) {
		sigaction(SIGINT, &oldint, NULL);
		sigaction(SIGQUIT, &oldquit, NULL);
		sigprocmask(SIG_SETMASK, &old, NULL);
		if (pid == 0) {
			execl("/bin/sh", "sh", "-c", cmd, (char *)0);
			_exit(127);
		}
		return -1;
	}
	while (waitpid(pid, &status, 0) == -1)
		if (errno != EINTR) {
			status = -1;
			break;
		}
	sigaction(SIGINT, &oldint, NULL);
	sigaction(SIGQUIT, &oldquit, NULL);
	sigprocmask(SIG_SETMASK, &old, NULL);
	return status;
}
