#include <sys/epoll.h>
#include "syscall.h"

int epoll_pwait(int fd, struct epoll_event *ev, int cnt, int to, const sigset_t *sigs)
{
	return syscall6(__NR_epoll_pwait, fd, (long)ev, cnt, to, (long)sigs, 8);
}
