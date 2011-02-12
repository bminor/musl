#include <unistd.h>
#include "syscall.h"

pid_t setsid(void)
{
	return syscall0(__NR_setsid);
}
