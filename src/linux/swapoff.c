#include <sys/swap.h>
#include "syscall.h"

int swapoff(const char *path)
{
	return syscall(SYS_swapoff, path);
}
