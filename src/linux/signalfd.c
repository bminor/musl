#include <sys/signalfd.h>
#include <signal.h>
#include "syscall.h"

int signalfd(int fd, const sigset_t *sigs, int flags)
{
	return syscall(SYS_signalfd, fd, sigs, _NSIG/8);
}
