#include <unistd.h>
#include "syscall.h"

pid_t getpgrp(void)
{
	return syscall0(__NR_getpgrp);
}
