#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int fexecve(int fd, char *const argv[], char *const envp[])
{
	static const char proc[] = "/proc/self/fd/%d";
	char buf[sizeof proc + 3*sizeof(int)];
	snprintf(buf, sizeof buf, proc, fd);
	execve(buf, argv, envp);
	if (errno == ENOENT) errno = EBADF;
	return -1;
}
