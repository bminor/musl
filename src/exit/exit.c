#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "libc.h"
#include "atomic.h"
#include "syscall.h"

static void dummy()
{
}

/* __towrite.c and atexit.c override these */
weak_alias(dummy, __funcs_on_exit);
weak_alias(dummy, __fflush_on_exit);

void exit(int code)
{
	static int lock;

	/* If more than one thread calls exit, hang until _Exit ends it all */
	while (a_swap(&lock, 1)) __syscall(SYS_pause);

	/* Only do atexit & stdio flush if they were actually used */
	__funcs_on_exit();
	__fflush_on_exit();

	/* Destructor s**t is kept separate from atexit to avoid bloat */
	if (libc.fini) libc.fini();
	if (libc.ldso_fini) libc.ldso_fini();

	_Exit(code);
	for(;;);
}
