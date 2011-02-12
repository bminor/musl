#include <sys/epoll.h>
#include "syscall.h"

int epoll_create1(int flags)
{
	return syscall1(__NR_epoll_create1, flags);
}
