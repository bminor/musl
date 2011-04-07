#include <stdlib.h>
#include "syscall.h"

void _Exit(int ec)
{
	__syscall(SYS_exit_group, ec);
	__syscall(SYS_exit, ec);
}
