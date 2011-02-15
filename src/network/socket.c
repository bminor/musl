#include <sys/socket.h>
#include "syscall.h"

int socket(int domain, int type, int protocol)
{
	return socketcall(socket, domain, type, protocol, 0, 0, 0);
}
