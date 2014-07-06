#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "syscall.h"
#include "libc.h"
#include "pthread_impl.h"

static void dummy(int x)
{
}

weak_alias(dummy, __fork_handler);

pid_t fork(void)
{
	pid_t ret;
	sigset_t set;
	__fork_handler(-1);
	__block_all_sigs(&set);
#ifdef SYS_fork
	ret = syscall(SYS_fork);
#else
	ret = syscall(SYS_clone, SIGCHLD, 0);
#endif
	if (libc.has_thread_pointer && !ret) {
		pthread_t self = __pthread_self();
		self->tid = __syscall(SYS_gettid);
		memset(&self->robust_list, 0, sizeof self->robust_list);
		libc.threads_minus_1 = 0;
	}
	__restore_sigs(&set);
	__fork_handler(!ret);
	return ret;
}
