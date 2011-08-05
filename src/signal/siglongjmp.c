#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

void siglongjmp(sigjmp_buf buf, int ret)
{
	unsigned long *flag = buf + sizeof(jmp_buf)/sizeof(long) - 1;
	sigset_t *mask = (void *)(flag + 1);
	if (*flag)
		sigprocmask (SIG_SETMASK, mask, NULL);
	longjmp((void *)buf, ret);
}
