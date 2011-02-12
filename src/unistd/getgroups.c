#include <unistd.h>
#include "syscall.h"

int getgroups(int count, gid_t list[])
{
	/* this depends on our gid_t being 32bit */
	return syscall2(__NR_getgroups32, count, (long)list);
}
