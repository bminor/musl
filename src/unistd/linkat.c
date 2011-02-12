#include <unistd.h>
#include "syscall.h"

int linkat(int fd1, const char *existing, int fd2, const char *new, int flag)
{
	return syscall5(__NR_linkat, fd1, (long)existing, fd2, (long)new, flag);
}
