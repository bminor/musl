#include <sys/mount.h>
#include "syscall.h"

int umount2(const char *special, int flags)
{
	return syscall(SYS_umount2, special, flags);
}
