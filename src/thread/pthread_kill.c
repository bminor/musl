#define SYSCALL_RETURN_ERRNO
#include "pthread_impl.h"

int pthread_kill(pthread_t t, int sig)
{
	return syscall3(__NR_tgkill, t->pid, t->tid, sig);
}
