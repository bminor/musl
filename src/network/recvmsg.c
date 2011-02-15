#include <sys/socket.h>
#include "syscall.h"
#include "libc.h"

ssize_t recvmsg(int fd, struct msghdr *msg, int flags)
{
	ssize_t r;
	CANCELPT_BEGIN;
	r = socketcall(recvmsg, fd, msg, flags, 0, 0, 0);
	CANCELPT_END;
	return r;
}
