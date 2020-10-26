#include <unistd.h>
#include <errno.h>
#include "libc.h"

static void dummy(int x) { }
weak_alias(dummy, __fork_handler);

pid_t fork(void)
{
	__fork_handler(-1);
	pid_t ret = _Fork();
	int errno_save = errno;
	__fork_handler(!ret);
	if (ret<0) errno = errno_save;
	return ret;
}
