#include <unistd.h>
#include "syscall.h"

int getgroups(int count, gid_t list[])
{
	return syscall2(__NR_getgroups, count, (long)list);
}
