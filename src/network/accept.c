#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"
#include "libc.h"

int accept(int fd, struct sockaddr *addr, socklen_t *len)
{
	unsigned long args[] = { fd, (unsigned long)addr, (unsigned long)len };
	int ret;
	CANCELPT_BEGIN;
	ret = syscall2(__NR_socketcall, SYS_ACCEPT, (long)args);
	CANCELPT_END;
	return ret;
}
