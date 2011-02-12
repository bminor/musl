#include <unistd.h>
#include "syscall.h"

int execve(const char *path, char *const argv[], char *const envp[])
{
	/* do we need to use environ if envp is null? */
	return syscall3(__NR_execve, (long)path, (long)argv, (long)envp);
}
