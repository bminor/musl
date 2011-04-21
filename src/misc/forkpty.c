#include <pty.h>
#include <unistd.h>

int forkpty(int *m, char *name, const struct termios *tio, const struct winsize *ws)
{
	int s;
	pid_t pid;

	if (openpty(m, &s, name, tio, ws) < 0) return -1;
	pid = fork();
	if (!pid) {
		close(*m);
		dup2(s, 0);
		dup2(s, 1);
		dup2(s, 2);
		close(s);
		return 0;
	}
	close(s);
	if (pid < 0) close(*m);
	return pid;
}
