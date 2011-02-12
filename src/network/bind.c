#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"

int bind(int fd, const struct sockaddr *addr, socklen_t len)
{
	unsigned long args[] = { fd, (unsigned long)addr, len };
	return syscall2(__NR_socketcall, SYS_BIND, (long)args);
}
