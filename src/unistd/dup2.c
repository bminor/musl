#include <unistd.h>
#include "syscall.h"

int dup2(int old, int new)
{
	return syscall(SYS_dup2, old, new);
}
