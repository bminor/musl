#include <sys/inotify.h>
#include "syscall.h"

int inotify_add_watch(int fd, const char *pathname, uint32_t mask)
{
	return syscall(SYS_inotify_add_watch, fd, pathname, mask);
}
