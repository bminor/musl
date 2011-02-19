#include <sys/inotify.h>
#include "syscall.h"

int inotify_init()
{
	return syscall0(__NR_inotify_init);
}
