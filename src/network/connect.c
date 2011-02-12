#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"
#include "libc.h"

int connect(int fd, const struct sockaddr *addr, socklen_t len)
{
	unsigned long args[] = { fd, (unsigned long)addr, len };
	int ret;
	CANCELPT_BEGIN;
	ret = syscall2(__NR_socketcall, SYS_CONNECT, (long)args);
	CANCELPT_END;
	return ret;
}
