#include <unistd.h>
#include "syscall.h"

int setgroups(int count, const gid_t list[])
{
	return syscall(SYS_setgroups, count, list);
}
