#include <signal.h>
#include "syscall.h"

int raise(int sig)
{
	return __syscall_kill(__syscall_getpid(), sig);
}
