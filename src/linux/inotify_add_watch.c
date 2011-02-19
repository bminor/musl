#include <sys/inotify.h>
#include "syscall.h"

int inotify_add_watch(int fd, const char *pathname, uint32_t mask)
{
	return syscall3(__NR_inotify_add_watch, fd, (long)pathname, mask);
}
