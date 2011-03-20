#include <sys/epoll.h>
#include "syscall.h"

int epoll_create(int size)
{
	return syscall(SYS_epoll_create, size);
}
