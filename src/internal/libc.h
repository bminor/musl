#ifndef LIBC_H
#define LIBC_H

#include <stdlib.h>
#include <stdio.h>

struct __libc {
	int *(*errno_location)(void);
	void (*cancelpt)(int);
	void (*lock)(volatile int *);
	void (*lockfile)(FILE *);
	void (*fork_handler)(int);
	int (*rsyscall)(int, long, long, long, long, long, long);
	int (*atexit)(void (*)(void));
	void (*fini)(void);
	void (*ldso_fini)(void);
	volatile int threads_minus_1;
	int ofl_lock;
	FILE *ofl_head;
};


#if 100*__GNUC__+__GNUC_MINOR__ >= 303 || defined(__PCC__) || defined(__TINYC__)
extern struct __libc __libc __attribute__((visibility("hidden")));
#define libc __libc

#elif !defined(__PIC__)
extern struct __libc __libc;
#define libc __libc

#else
#define USE_LIBC_ACCESSOR
extern struct __libc *__libc_loc(void) __attribute__((const));
#define libc (*__libc_loc())

#endif


/* Designed to avoid any overhead in non-threaded processes */
void __lock(volatile int *);
void __lockfile(FILE *);
#define LOCK(x) (libc.threads_minus_1 ? (__lock(x),1) : ((void)(x),1))
#define UNLOCK(x) (*(x)=0)
#define CANCELPT(x) (libc.cancelpt ? libc.cancelpt((x)),0 : (void)(x),0)
#define CANCELPT_BEGIN CANCELPT(1)
#define CANCELPT_TRY CANCELPT(0)
#define CANCELPT_END CANCELPT(-1)
#define CANCELPT_INHIBIT CANCELPT(2)
#define CANCELPT_RESUME CANCELPT(-2)

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
