#include "stdio_impl.h"

int pclose(FILE *f)
{
	int status;
	pid_t pid = f->pipe_pid;
	fclose(f);
	while (waitpid(pid, &status, 0) == -1)
		if (errno != EINTR) return -1;
	return status;
}
