#include <unistd.h>
#include "syscall.h"

int setgroups(int count, const gid_t list[])
{
	return syscall2(__NR_setgroups, count, (long)list);
}
