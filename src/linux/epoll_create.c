#include <sys/epoll.h>
#include "syscall.h"

int epoll_create(int size)
{
	return syscall1(__NR_epoll_create, size);
}
