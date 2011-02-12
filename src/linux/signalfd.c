#include <sys/signalfd.h>
#include "syscall.h"

int signalfd(int fd, const sigset_t *sigs, int flags)
{
	return syscall3(__NR_signalfd, fd, (long)sigs, 8);
}
