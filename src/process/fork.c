#include <unistd.h>
#include "syscall.h"
#include "libc.h"
#include "pthread_impl.h"

pid_t fork(void)
{
	pid_t ret;
	if (libc.fork_handler) libc.fork_handler(-1);
	ret = syscall(SYS_fork);
	if (libc.main_thread && !ret) {
		pthread_t self = __pthread_self();
		self->tid = self->pid = syscall(SYS_getpid);
		memset(&self->robust_list, 0, sizeof self->robust_list);
		libc.threads_minus_1 = 0;
		libc.main_thread = self;
	}
	if (libc.fork_handler) libc.fork_handler(!ret);
	return ret;
}
