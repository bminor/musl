#include <sys/inotify.h>
#include "syscall.h"

int inotify_init1(int flags)
{
	return syscall1(__NR_inotify_init1, flags);
}
