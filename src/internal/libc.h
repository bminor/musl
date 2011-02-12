#ifndef LIBC_H
#define LIBC_H

#include <stdlib.h>
#include <stdio.h>

#define libc __libc
extern struct libc {
	void (*lock)(volatile int *);
	void (*cancelpt)(int);
	int (*atexit)(void (*)(void));
	void (*fini)(void);
	void (*ldso_fini)(void);
	int *(*errno_location)(void);
	volatile int threads_minus_1;
	int (*rsyscall)(int, long, long, long, long, long, long);
	void (**tsd_keys)(void *);
} libc;


/* Designed to avoid any overhead in non-threaded processes */
void __lock(volatile int *);
#define LOCK(x) (libc.threads_minus_1 ? (__lock(x),1) : ((void)(x),1))
#define UNLOCK(x) (*(x)=0)
#define CANCELPT(x) (libc.cancelpt ? libc.cancelpt((x)),0 : (void)(x),0)
#define CANCELPT_BEGIN CANCELPT(1)
#define CANCELPT_END CANCELPT(0)

extern char **__environ;
#define environ __environ

#undef weak_alias
#define weak_alias(old, new) \
	extern __typeof(old) new __attribute__((weak, alias(#old)))

#undef LFS64_2
//#define LFS64_2(x, y) weak_alias(x, y)
#define LFS64_2(x, y) extern __typeof(x) y

#undef LFS64
#define LFS64(x) LFS64_2(x, x##64)

#endif
