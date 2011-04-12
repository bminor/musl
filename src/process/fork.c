#include <unistd.h>
#include "syscall.h"
#include "libc.h"
#include "pthread_impl.h"

pid_t fork(void)
{
	pid_t ret;
	if (libc.fork_handler) libc.fork_handler(-1);
	ret = syscall(SYS_fork);
	if (libc.lock && !ret) {
		pthread_t self = __pthread_self();
		self->tid = self->pid = syscall(SYS_getpid);
		libc.threads_minus_1 = 0;
	}
	if (libc.fork_handler) libc.fork_handler(!ret);
	return ret;
}
