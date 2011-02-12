#include <unistd.h>
#include "syscall.h"

int unlink(const char *path)
{
	return __syscall_unlink(path);
}
