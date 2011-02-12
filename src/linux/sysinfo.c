#define SYSCALL_STANDALONE
#include "syscall.h"

struct sysinfo;

int sysinfo(struct sysinfo *info)
{
	return syscall1(__NR_sysinfo, (long)info);
}
