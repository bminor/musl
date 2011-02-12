#include <unistd.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int chroot(const char *path)
{
	return syscall1(__NR_chroot, (long)path);
}
