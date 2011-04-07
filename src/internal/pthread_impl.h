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
	struct pthread *self;
	unsigned long tlsdesc[4];
	pid_t tid, pid;
	int tsd_used, errno_val, *errno_ptr;
	volatile int canceldisable, cancelasync, cancelpoint, cancel;
	unsigned char *map_base;
	size_t map_size;
	void *start_arg;
	void *(*start)(void *);
	void *result;
	int detached;
	int exitlock;
	struct __ptcb *cancelbuf;
	void **tsd;
	pthread_attr_t attr;
	volatile int dead;
	struct {
		void **head;
		long off;
		void *pending;
	} robust_list;
	int unblock_cancel;
};

struct __timer {
	int timerid;
	pthread_t thread;
};

#define __SU (sizeof(size_t)/sizeof(int))

#define _a_stacksize __u.__s[0]
#define _a_guardsize __u.__s[1]
#define _a_detach __u.__i[2*__SU+0]
#define _m_type __u.__i[0]
#define _m_lock __u.__i[1]
#define _m_waiters __u.__i[2]
#define _m_prev __u.__p[3]
#define _m_next __u.__p[4]
#define _m_count __u.__i[5]
#define _c_block __u.__i[0]
#define _c_clock __u.__i[1]
#define _rw_wrlock __u.__i[0]
#define _rw_readers __u.__i[1]
#define _rw_waiters __u.__i[2]
#define _rw_owner __u.__i[3]
#define _b_count __u.__i[0]
#define _b_limit __u.__i[1]
#define _b_left __u.__i[2]
#define _b_waiters __u.__i[3]

#include "pthread_arch.h"

#define SIGCANCEL 32
#define SIGSYSCALL 33

int __set_thread_area(void *);
int __libc_sigaction(int, const struct sigaction *, struct sigaction *);
int __libc_sigprocmask(int, const sigset_t *, sigset_t *);
void __lock(volatile int *);
void __unmapself(void *, size_t);

int __timedwait(volatile int *, int, clockid_t, const struct timespec *, int);
void __wait(volatile int *, volatile int *, int, int);
void __wake(volatile int *, int, int);

void __rsyscall_lock();
void __rsyscall_unlock();

#define DEFAULT_STACK_SIZE (16384-PAGE_SIZE)
#define DEFAULT_GUARD_SIZE PAGE_SIZE

#endif
