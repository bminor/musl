#include <unistd.h>
#include "syscall.h"
#include "libc.h"

int pause(void)
{
	int r;
	CANCELPT_BEGIN;
	r = syscall0(__NR_pause);
	CANCELPT_END;
	return r;
}
