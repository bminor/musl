#include <sys/socket.h>
#include "syscall.h"

int getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen)
{
	return socketcall(getsockopt, fd, level, optname, optval, optlen, 0);
}
