#include <sys/mount.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int umount(const char *special)
{
	return syscall1(__NR_umount, (long)special);
}
