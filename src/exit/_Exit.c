#include <stdlib.h>
#include "syscall.h"

void _Exit(int ec)
{
	__syscall(__NR_exit_group, ec);
	__syscall(__NR_exit, ec);
}
