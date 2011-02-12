#include <sys/socket.h>
#include "syscall.h"
#include "socketcall.h"

int socket(int domain, int type, int protocol)
{
	unsigned long args[] = { domain, type, protocol };
	return syscall2(__NR_socketcall, SYS_SOCKET, (long)args);
}
