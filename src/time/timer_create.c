#include <time.h>
#include "pthread_impl.h"

struct ksigevent {
	union sigval sigev_value;
	int sigev_signo;
	int sigev_notify;
	int sigev_tid;
};

struct start_args {
	pthread_barrier_t b;
	struct sigevent *sev;
	timer_t t;
};

static void sighandler(int sig, siginfo_t *si, void *ctx)
{
	int st;
	timer_t t = si->si_value.sival_ptr;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &st);
	t->notify(t->val);
	pthread_setcancelstate(st, 0);
}

static void killtimer(void *arg)
{
	timer_t t = arg;
	if (t->timerid >= 0) __syscall(SYS_timer_delete, t->timerid);
}

static void *start(void *arg)
{
	struct start_args *args = arg;
	struct __timer t = {
		.notify = args->sev->sigev_notify_function,
		.val = args->sev->sigev_value,
	};

	args->t = &t;

	pthread_barrier_wait(&args->b);

	pthread_cleanup_push(killtimer, &t);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	/* Loop on async-signal-safe cancellation point */
	for (;;) sleep(1);
	pthread_cleanup_pop(1);
	return 0;
}

int timer_create(clockid_t clk, struct sigevent *evp, timer_t *res)
{
	struct sigevent sev = { 
		.sigev_notify = SIGEV_SIGNAL,
		.sigev_signo = SIGALRM
	};
	pthread_t td;
	pthread_attr_t attr;
	int r;
	struct start_args args;
	timer_t t;
	struct ksigevent ksev;

	if (evp) sev = *evp;

	switch (sev.sigev_notify) {
	case SIGEV_NONE:
	case SIGEV_SIGNAL:
		if (!(t = calloc(1, sizeof *t)))
			return -1;
		ksev.sigev_value = evp ? sev.sigev_value
			: (union sigval){.sival_ptr=t};
		ksev.sigev_signo = sev.sigev_signo;
		ksev.sigev_notify = sev.sigev_notify;
		ksev.sigev_tid = 0;
		break;
	case SIGEV_THREAD:
		if (sev.sigev_notify_attributes)
			attr = *sev.sigev_notify_attributes;
		else
			pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_barrier_init(&args.b, 0, 2);
		args.sev = &sev;
		r = pthread_create(&td, &attr, start, &args);
		if (r) {
			errno = r;
			return -1;
		}
		pthread_barrier_wait(&args.b);
		t = args.t;
		t->thread = td;
		ksev.sigev_value.sival_ptr = t;
		ksev.sigev_signo = SIGCANCEL;
		ksev.sigev_notify = 4; /* SIGEV_THREAD_ID */
		ksev.sigev_tid = td->tid;
		if (!libc.sigtimer) libc.sigtimer = sighandler;
		break;
	}

	t->timerid = -1;
	if (syscall(SYS_timer_create, clk, &ksev, &t->timerid) < 0) {
		timer_delete(t);
		return -1;
	}

	*res = t;
	return 0;
}
