#include <sys/swap.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int swapoff(const char *path)
{
	return syscall1(__NR_swapoff, (long)path);
}
