#include <sys/socket.h>
#include "syscall.h"
#include "libc.h"

ssize_t recvfrom(int fd, void *buf, size_t len, int flags, struct sockaddr *addr, socklen_t *alen)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = socketcall(recvfrom, fd, buf, len, flags, addr, alen);
	CANCELPT_END;
	return r;
}
