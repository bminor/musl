#include <signal.h>
#include "syscall.h"

int sigaltstack(const stack_t *ss, stack_t *old)
{
	/* depends on kernel struct matching */
	return syscall2(__NR_sigaltstack, (long)ss, (long)old);
}
