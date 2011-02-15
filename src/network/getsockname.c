#include <sys/socket.h>
#include "syscall.h"

int getsockname(int fd, struct sockaddr *addr, socklen_t *len)
{
	return socketcall(getsockname, fd, addr, len, 0, 0, 0);
}
