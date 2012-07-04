#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>
#include "syscall.h"

void siglongjmp(sigjmp_buf buf, int ret)
{
	if (buf->__fl)
		__syscall(SYS_rt_sigprocmask, SIG_SETMASK, buf->__ss, 0, 8);
	longjmp(buf->__jb, ret);
}
