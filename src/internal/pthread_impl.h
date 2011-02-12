#ifndef _PTHREAD_IMPL_H
#define _PTHREAD_IMPL_H

#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include <inttypes.h>
#include <setjmp.h>
#include <string.h>
#include <time.h>
#include "libc.h"
#include "syscall.h"
#include "atomic.h"
#include "futex.h"

#define pthread __pthread

struct pthread {
	struct pthread *self, *join;
	int errno_val;
	pid_t tid, pid;
	volatile int canceldisable, cancelasync, cancelpoint, cancel;
	unsigned char *map_base;
	size_t map_size;
	void *start_arg;
	void *(*start)(void *);
	void *result;
	jmp_buf exit_jmp_buf;
	int detached;
	int exitlock;
	unsigned long tlsdesc[4];
	struct __ptcb *cancelbuf;
	void **tsd;
	int tsd_used;
	pthread_attr_t attr;
	int *errno_ptr;
};

static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ ("movl %%gs:0,%0" : "=r" (self) );
	return self;
}

#define SIGCANCEL 32
#define SIGSYSCALL 33
#define SIGTIMER  32 /* ?? */

int __set_thread_area(unsigned long *);
int __set_pthread_self(void *);
int __libc_sigaction(int, const struct sigaction *, struct sigaction *);
int __libc_sigprocmask(int, const sigset_t *, sigset_t *);
void __lock(volatile int *);
void __unmapself(void *, size_t);

int __timedwait(volatile int *, int, clockid_t, const struct timespec *, int);
void __wait(volatile int *, volatile int *, int, int);
void __wake(volatile int *, int, int);

#define DEFAULT_STACK_SIZE (16384-PAGE_SIZE)
#define DEFAULT_GUARD_SIZE PAGE_SIZE

#endif
