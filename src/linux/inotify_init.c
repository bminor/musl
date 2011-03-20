#include <sys/inotify.h>
#include "syscall.h"

int inotify_init()
{
	return syscall(SYS_inotify_init);
}
