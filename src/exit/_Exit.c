#include <stdlib.h>
#define SYSCALL_NORETURN
#include "syscall.h"

void _Exit(int ec)
{
	syscall1(__NR_exit_group, ec);
	syscall1(__NR_exit, ec);
}
