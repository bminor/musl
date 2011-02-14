#include <sys/mount.h>
#include "syscall.h"

int umount(const char *special)
{
	return syscall2(__NR_umount2, (long)special, 0);
}
