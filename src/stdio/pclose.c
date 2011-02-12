#include "stdio_impl.h"

int pclose(FILE *f)
{
	int status;
	fclose(f);
	while (waitpid(f->pipe_pid, &status, 0) == -1)
		if (errno != EINTR) return -1;
	return status;
}
