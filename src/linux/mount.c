#include <sys/mount.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int mount(const char *special, const char *dir, const char *fstype, unsigned long flags, const void *data)
{
	return syscall5(__NR_mount, (long)special, (long)dir, (long)fstype, flags, (long)data);
}
