#include <unistd.h>
#include "syscall.h"
#include "libc.h"
#include "pthread_impl.h"

pid_t fork(void)
{
	pid_t ret;
	if (libc.fork_handler) libc.fork_handler(-1);
	ret = syscall0(__NR_fork);
	if (libc.lock && !ret) {
		pthread_t self = __pthread_self();
		self->pid = syscall0(__NR_getpid);
		self->tid = syscall0(__NR_gettid);
		libc.threads_minus_1 = 0;
	}
	if (libc.fork_handler) libc.fork_handler(!ret);
	return ret;
}
