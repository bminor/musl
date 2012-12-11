#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "syscall.h"

int pipe2(int fd[2], int flag)
{
	if (!flag) return syscall(SYS_pipe, fd);
	int ret = __syscall(SYS_pipe2, fd, flag);
	if (ret != -ENOSYS) return __syscall_ret(ret);
	ret = __syscall(SYS_pipe, fd);
	if (ret) return __syscall_ret(ret);
	if (flag & O_CLOEXEC) {
		fcntl(fd[0], F_SETFD, FD_CLOEXEC);
		fcntl(fd[1], F_SETFD, FD_CLOEXEC);
	}
	if (flag & O_NONBLOCK) {
		fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
		fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
	}
	return 0;
}
