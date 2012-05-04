#include <sched.h>
#include "syscall.h"

int sched_setparam(pid_t pid, const struct sched_param *param)
{
	static const struct sched_param def;
	return syscall(SYS_sched_setparam, pid, &def);
}
