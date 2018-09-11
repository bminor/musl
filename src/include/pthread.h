#ifndef PTHREAD_H
#define PTHREAD_H

#include "../../include/pthread.h"

int __pthread_once(pthread_once_t *, void (*)(void));
void __pthread_testcancel(void);
int __pthread_setcancelstate(int, int *);
int __pthread_create(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void *), void *restrict);
_Noreturn void __pthread_exit(void *);
int __pthread_join(pthread_t, void **);
int __pthread_mutex_lock(pthread_mutex_t *);
int __pthread_mutex_trylock(pthread_mutex_t *);
int __pthread_mutex_trylock_owner(pthread_mutex_t *);
int __pthread_mutex_timedlock(pthread_mutex_t *restrict, const struct timespec *restrict);
int __pthread_mutex_unlock(pthread_mutex_t *);
int __private_cond_signal(pthread_cond_t *, int);
int __pthread_cond_timedwait(pthread_cond_t *restrict, pthread_mutex_t *restrict, const struct timespec *restrict);
int __pthread_key_create(pthread_key_t *, void (*)(void *));
int __pthread_key_delete(pthread_key_t);

#endif
