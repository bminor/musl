#include <sys/epoll.h>
#include "syscall.h"

int epoll_ctl(int fd, int op, int fd2, struct epoll_event *ev)
{
	return syscall4(__NR_epoll_ctl, fd, op, fd2, (long)ev);
}
