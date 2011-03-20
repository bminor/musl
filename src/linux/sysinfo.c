#include "syscall.h"

struct sysinfo;

int sysinfo(struct sysinfo *info)
{
	return syscall(SYS_sysinfo, info);
}
