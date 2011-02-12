#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"

int socketpair(int domain, int type, int protocol, int fd[2])
{
	unsigned long args[] = { domain, type, protocol, (unsigned long)fd };
	return syscall2(__NR_socketcall, SYS_SOCKETPAIR, (long)args);
}
