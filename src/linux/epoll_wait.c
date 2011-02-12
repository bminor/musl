#include <sys/epoll.h>
#include "syscall.h"

int epoll_wait(int fd, struct epoll_event *ev, int cnt, int to)
{
	return syscall4(__NR_epoll_wait, fd, (long)ev, cnt, to);
}
