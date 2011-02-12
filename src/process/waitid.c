#include <sys/wait.h>
#include "syscall.h"

int waitid(idtype_t type, id_t id, siginfo_t *info, int options)
{
	return syscall5(__NR_waitid, type, id, (long)info, options, 0);
}
