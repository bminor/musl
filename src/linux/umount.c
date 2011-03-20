#include <sys/mount.h>
#include "syscall.h"

int umount(const char *special)
{
	return syscall(SYS_umount2, special, 0);
}
