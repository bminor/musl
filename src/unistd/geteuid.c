#include <unistd.h>
#include "syscall.h"

uid_t geteuid(void)
{
	return syscall0(__NR_geteuid32);
}
