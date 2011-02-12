#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "libc.h"

/* __overflow.c and atexit.c override these */
static int (*const dummy)() = 0;
weak_alias(dummy, __funcs_on_exit);
weak_alias(dummy, __fflush_on_exit);

void exit(int code)
{
	static int lock;

	/* If more than one thread calls exit, hang until _Exit ends it all */
	LOCK(&lock);

	/* Only do atexit & stdio flush if they were actually used */
	if (__funcs_on_exit) __funcs_on_exit();
	if (__fflush_on_exit) __fflush_on_exit(0);

	/* Destructor s**t is kept separate from atexit to avoid bloat */
	if (libc.fini) libc.fini();
	if (libc.ldso_fini) libc.ldso_fini();

	_Exit(code);
	for(;;);
}
