#ifndef _PTHREAD_H
#define _PTHREAD_H
#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_time_t
#define __NEED_struct_timespec
#define __NEED_sigset_t
#define __NEED_pthread_t
#define __NEED_size_t

#include <bits/alltypes.h>

struct sched_param;

typedef int pthread_once_t, pthread_key_t, pthread_spinlock_t;
typedef int pthread_mutexattr_t, pthread_condattr_t, pthread_barrierattr_t;

typedef struct {
	size_t __guardsize;
	size_t __stacksize;
	unsigned __detach : 1;
	unsigned __pad : 31;
	int __attr[6];
} pthread_attr_t;

typedef struct {
	int __attr[2];
} pthread_rwlockattr_t;

typedef struct {
	int __type;
	int __lock;
	pthread_t __owner;
	int __pad2;
	int __waiters;
	int __pad;
} pthread_mutex_t;

typedef struct {
	int __block;
	int __pad[11];
} pthread_cond_t;

typedef struct {
	int __wrlock;
	int __readers;
	int __waiters;
	int __owner;
	int __pad[4];
} pthread_rwlock_t;

typedef struct {
	int __count;
	int __limit;
	int __left;
	int __waiters;
	int __barrier[1];
} pthread_barrier_t;

#define PTHREAD_CREATE_JOINABLE 0
#define PTHREAD_CREATE_DETACHED 1

#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_DEFAULT 0
#define PTHREAD_MUTEX_RECURSIVE 1
#define PTHREAD_MUTEX_ERRORCHECK 2

#define PTHREAD_MUTEX_STALLED 0
#define PTHREAD_MUTEX_ROBUST 1

#define PTHREAD_PRIO_NONE 0
#define PTHREAD_PRIO_INHERIT 1
#define PTHREAD_PRIO_PROTECT 2

#define PTHREAD_INHERIT_SCHED 0
#define PTHREAD_EXPLICIT_SCHED 1

#define PTHREAD_SCOPE_SYSTEM 0
#define PTHREAD_SCOPE_PROCESS 1

#define PTHREAD_PROCESS_PRIVATE 0
#define PTHREAD_PROCESS_SHARED 1


#define PTHREAD_MUTEX_INITIALIZER {0}
#define PTHREAD_RWLOCK_INITIALIZER {0}
#define PTHREAD_COND_INITIALIZER {0}
#define PTHREAD_ONCE_INIT 0


#define PTHREAD_CANCEL_ENABLE 0
#define PTHREAD_CANCEL_DISABLE 1

#define PTHREAD_CANCEL_DEFERRED 0
#define PTHREAD_CANCEL_ASYNCHRONOUS 1

#define PTHREAD_CANCELLED ((void *)-1)


#define PTHREAD_BARRIER_SERIAL_THREAD (-1)


int pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
int pthread_detach(pthread_t);
void pthread_exit(void *);
int pthread_join(pthread_t, void **);

pthread_t pthread_self(void);
int pthread_equal(pthread_t, pthread_t);

int pthread_setcancelstate(int, int *);
int pthread_setcanceltype(int, int *);
void pthread_testcancel(void);
int pthread_cancel(pthread_t);

int pthread_once(pthread_once_t *, void (*)(void));

int pthread_mutex_init(pthread_mutex_t *, const pthread_mutexattr_t *);
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);
int pthread_mutex_trylock(pthread_mutex_t *);
int pthread_mutex_destroy(pthread_mutex_t *);

int pthread_cond_init(pthread_cond_t *, const pthread_condattr_t *);
int pthread_cond_destroy(pthread_cond_t *);
int pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
int pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *, const struct timespec *);
int pthread_cond_broadcast(pthread_cond_t *);
int pthread_cond_signal(pthread_cond_t *);

int pthread_rwlock_init(pthread_rwlock_t *, const pthread_rwlockattr_t *);
int pthread_rwlock_destroy(pthread_rwlock_t *);
int pthread_rwlock_rdlock(pthread_rwlock_t *);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *, const struct timespec *);
int pthread_rwlock_wrlock(pthread_rwlock_t *);
int pthread_rwlock_trywrlock(pthread_rwlock_t *);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *, const struct timespec *);
int pthread_rwlock_unlock(pthread_rwlock_t *);

int pthread_spin_init(pthread_spinlock_t *, int);
int pthread_spin_destroy(pthread_spinlock_t *);
int pthread_spin_lock(pthread_spinlock_t *);
int pthread_spin_trylock(pthread_spinlock_t *);
int pthread_spin_unlock(pthread_spinlock_t *);

int pthread_barrier_init(pthread_barrier_t *, const pthread_barrierattr_t *, unsigned);
int pthread_barrier_destroy(pthread_barrier_t *);
int pthread_barrier_wait(pthread_barrier_t *);

int pthread_key_create(pthread_key_t *, void (*)(void *));
int pthread_key_delete(pthread_key_t);
void *pthread_getspecific(pthread_key_t);
int pthread_setspecific(pthread_key_t, const void *);

int pthread_attr_init(pthread_attr_t *);
int pthread_attr_destroy(pthread_attr_t *);

int pthread_attr_getguardsize(pthread_attr_t *, size_t *);
int pthread_attr_setguardsize(pthread_attr_t *, size_t);
int pthread_attr_getstacksize(pthread_attr_t *, size_t *);
int pthread_attr_setstacksize(pthread_attr_t *, size_t);
int pthread_attr_getdetachstate(pthread_attr_t *, int *);
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_attr_getstack(pthread_attr_t *, void **, size_t *);
int pthread_attr_setstack(pthread_attr_t *, void *, size_t);
int pthread_attr_getscope(pthread_attr_t *, int *);
int pthread_attr_setscope(pthread_attr_t *, int);
int pthread_attr_getschedpolicy(pthread_attr_t *, int *);
int pthread_attr_setschedpolicy(pthread_attr_t *, int);
int pthread_attr_getschedparam(pthread_attr_t *, struct sched_param *);
int pthread_attr_setschedparam(pthread_attr_t *, const struct sched_param *);
int pthread_attr_getinheritsched(pthread_attr_t *, int *);
int pthread_attr_setinheritsched(pthread_attr_t *, int);

int pthread_mutexattr_destroy(pthread_mutexattr_t *);
int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *, int *);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *, int *);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *, int *);
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *, int *);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *, int *);
int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
int pthread_mutexattr_setrobust(pthread_mutexattr_t *, int);
int pthread_mutexattr_settype(pthread_mutexattr_t *, int);

int pthread_barrierattr_destroy(pthread_barrierattr_t *);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *, int *);
int pthread_barrierattr_init(pthread_barrierattr_t *);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *, int);

#include <bits/pthread.h>

int __setjmp(void *);
void __pthread_register_cancel(struct __ptcb *);
void __pthread_unregister_cancel(struct __ptcb *);
void __pthread_unwind_next(struct __ptcb *);

#define pthread_cleanup_push(f, x) \
do { struct __ptcb __cb; void (*__f)(void *) = (f); void *__x = (x); \
if (__setjmp(__cb.__jb)) __f(__x), __pthread_unwind_next(&__cb); \
__pthread_register_cancel(&__cb); {

#define pthread_cleanup_pop(r) ; } \
__pthread_unregister_cancel(&__cb); \
if (r) __f(__x); } while (0)


#ifdef __cplusplus
}
#endif
#endif
