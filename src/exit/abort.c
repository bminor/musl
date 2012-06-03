#include <stdlib.h>
#include <signal.h>
#include "syscall.h"

void abort(void)
{
	raise(SIGABRT);
	raise(SIGKILL);
	for (;;);
}
