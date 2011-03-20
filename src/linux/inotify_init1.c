#include <sys/inotify.h>
#include "syscall.h"

int inotify_init1(int flags)
{
	return syscall(SYS_inotify_init1, flags);
}
