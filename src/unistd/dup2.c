#include <unistd.h>
#include "syscall.h"

int dup2(int old, int new)
{
	return __syscall_dup2(old, new);
}
