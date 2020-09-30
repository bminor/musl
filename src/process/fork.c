#include <unistd.h>
#include "libc.h"

static void dummy(int x) { }
weak_alias(dummy, __fork_handler);

pid_t fork(void)
{
	__fork_handler(-1);
	pid_t ret = _Fork();
	__fork_handler(!ret);
	return ret;
}
