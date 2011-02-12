#include <signal.h>
#include "syscall.h"

int kill(pid_t pid, int sig)
{
	return syscall2(__NR_kill, pid, sig);
}
