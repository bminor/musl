#define SYSCALL_STANDALONE
#include "syscall.h"

int klogctl (int type, char *buf, int len)
{
	return syscall3(__NR_syslog, type, (long)buf, len);
}
