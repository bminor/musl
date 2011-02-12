#define SYSCALL_RETURN_ERRNO
#include "pthread_impl.h"

int pthread_cancel(pthread_t t)
{
	return syscall3(__NR_tgkill, t->pid, t->tid, SIGCANCEL);
}
