#define _GNU_SOURCE
#include <unistd.h>
#include "syscall.h"

int pipe2(int fd[2], int flg)
{
	return syscall(SYS_pipe2, fd, flg);
}
