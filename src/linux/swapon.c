#include <sys/swap.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int swapon(const char *path, int flags)
{
	return syscall2(__NR_swapon, (long)path, flags);
}
