#include <unistd.h>
#include "syscall.h"

int symlinkat(const char *existing, int fd, const char *new)
{
	return syscall3(__NR_symlinkat, (long)existing, fd, (long)new);
}
