#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

void siglongjmp(sigjmp_buf buf, int ret)
{
	long *flag = buf + sizeof(jmp_buf)/sizeof(long);
	sigset_t *mask = (void *)(flag + 1);
	if (*flag)
		sigprocmask (SIG_SETMASK, mask, NULL);
	longjmp((void *)buf, ret);
}
