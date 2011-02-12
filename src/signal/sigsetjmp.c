#include <setjmp.h>
#include <signal.h>
#include <stdlib.h>

/* !!! This function will not work unless the compiler performs
 * tail call optimization. Machine-specific asm versions should
 * be created instead even though the workaround (tail call)
 * is entirely non-machine-specific... */

int sigsetjmp(sigjmp_buf buf, int save)
{
	long *flag = buf + sizeof(jmp_buf)/sizeof(long);
	sigset_t *mask = (void *)(flag + 1);
	if ((*flag = save))
		sigprocmask (SIG_SETMASK, NULL, mask);
	return setjmp((void *)buf);
}
