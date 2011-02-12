#include <unistd.h>
#include "syscall.h"

int symlink(const char *existing, const char *new)
{
	return syscall2(__NR_symlink, (long)existing, (long)new);
}
