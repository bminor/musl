#include <unistd.h>
#include "syscall.h"

pid_t setpgid(pid_t pid, pid_t pgid)
{
	return syscall(SYS_setpgid, pid, pgid);
}
