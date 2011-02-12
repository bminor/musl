#include <sys/mount.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int umount2(const char *special, int flags)
{
	return syscall2(__NR_umount2, (long)special, flags);
}
