#ifndef _SIGNAL_H
#define _SIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size_t
#define __NEED_pid_t
#define __NEED_uid_t
#define __NEED_struct_timespec
#define __NEED_pthread_t
#define __NEED_time_t
#define __NEED_clock_t
#define __NEED_sigset_t
#define __NEED_siginfo_t

#include <bits/alltypes.h>

typedef int sig_atomic_t;

struct sigaction
{
	union {
		void (*sa_handler)(int);
		void (*sa_sigaction)(int, siginfo_t *, void *);
	} __sa_handler;
	sigset_t sa_mask;
	int sa_flags;
	void (*__sa_restorer)(void);	
};
#define sa_handler   __sa_handler.sa_handler
#define sa_sigaction __sa_handler.sa_sigaction

typedef struct
{
	void *ss_sp;
	int ss_flags;
	size_t ss_size;
} stack_t;

union sigval
{
	int sival_int;
	void *sival_ptr;
};

#include <bits/signal.h>

int __libc_current_sigrtmin(void);
int __libc_current_sigrtmax(void);

#define SIGRTMIN  (__libc_current_sigrtmin())
#define SIGRTMAX  (__libc_current_sigrtmax())

void (*signal(int, void (*)(int)))(int);
void (*bsd_signal(int, void (*)(int)))(int);
int kill(pid_t, int);
int killpg(pid_t, int);
int raise(int);
int sigpause(int);

int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sigaddset(sigset_t *, int);
int sigdelset(sigset_t *, int);
int sigismember(const sigset_t *, int);

int sigprocmask(int, const sigset_t *, sigset_t *);
int sigsuspend(const sigset_t *);
int sigaction(int, const struct sigaction *, struct sigaction *);
int sigpending(sigset_t *);
int sigwait(const sigset_t *, int *);
int sigwaitinfo(const sigset_t *, siginfo_t *);
int sigtimedwait(const sigset_t *, siginfo_t *, const struct timespec *);
int sigqueue(pid_t, int, const union sigval);

int siginterrupt(int, int);

int sigaltstack(const stack_t *, stack_t *);

int sighold(int);
int sigrelse(int);
int sigignore(int);
void (*sigset(int, void (*)(int)))(int);

int pthread_sigmask(int, const sigset_t *, sigset_t *);
int pthread_kill(pthread_t, int);

#ifdef _GNU_SOURCE
typedef int (*sighandler_t)(int);
#endif

#ifdef __cplusplus
}
#endif

#endif
