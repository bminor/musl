#include <unistd.h>
#include "syscall.h"

int sethostname(const char *name, size_t len)
{
	return syscall2(__NR_sethostname, (long)name, len);
}
