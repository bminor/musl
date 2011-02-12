#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"
#include "libc.h"

ssize_t recvmsg(int fd, struct msghdr *msg, int flags)
{
	unsigned long args[] = { fd, (unsigned long)msg, flags };
	ssize_t r;
	CANCELPT_BEGIN;
	r = syscall2(__NR_socketcall, SYS_RECVMSG, (long)args);
	CANCELPT_END;
	return r;
}
