#include <unistd.h>
#include "syscall.h"

int pipe(int fd[2])
{
	return syscall1(__NR_pipe, (long)fd);
}
