#include <unistd.h>
#include "syscall.h"

int link(const char *existing, const char *new)
{
	return syscall2(__NR_link, (long)existing, (long)new);
}
