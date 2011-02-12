#include <unistd.h>
#include "syscall.h"

int fchown(int fd, uid_t uid, gid_t gid)
{
	return syscall3(__NR_fchown32, fd, uid, gid);
}
