#include <sys/socket.h>
#include "syscall.h"
#include "libc.h"

int accept(int fd, struct sockaddr *addr, socklen_t *len)
{
	return socketcall_cp(accept, fd, addr, len, 0, 0, 0);
}
