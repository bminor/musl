#include <unistd.h>
#define SYSCALL_STANDALONE
#include "syscall.h"

int setgroups(int count, const gid_t list[])
{
	/* this depends on our gid_t being 32bit */
	return syscall2(__NR_setgroups32, count, (long)list);
}
