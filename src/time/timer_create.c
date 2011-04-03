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
};

void __sigtimer_handler(pthread_t self)
{
	int st;
	void (*notify)(union sigval) = (void (*)(union sigval))self->start;
	union sigval val = { .sival_ptr = self->start_arg };

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &st);
	notify(val);
	pthread_setcancelstate(st, 0);
}

static void cleanup(void *p)
{
	pthread_t self = p;
	__syscall(SYS_timer_delete, self->result);
}

static void *start(void *arg)
{
	pthread_t self = __pthread_self();
	struct start_args *args = arg;

	/* Reuse no-longer-needed thread structure fields to avoid
	 * needing the timer address in the signal handler. */
	self->start = (void *(*)(void *))args->sev->sigev_notify_function;
	self->start_arg = args->sev->sigev_value.sival_ptr;
	self->result = (void *)-1;

	pthread_cleanup_push(cleanup, self);
	pthread_barrier_wait(&args->b);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	/* Loop on async-signal-safe cancellation point */
	for (;;) sleep(1000000000);
	pthread_cleanup_pop(0);
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
	int timerid;

	if (evp) sev = *evp;

	switch (sev.sigev_notify) {
	case SIGEV_NONE:
	case SIGEV_SIGNAL:
		ksev.sigev_value = evp ? sev.sigev_value
			: (union sigval){.sival_ptr=t};
		ksev.sigev_signo = sev.sigev_signo;
		ksev.sigev_notify = sev.sigev_notify;
		ksev.sigev_tid = 0;
		if (syscall(SYS_timer_create, clk, &ksev, &timerid) < 0)
			return -1;
		*res = (void *)(2*timerid+1);
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
		ksev.sigev_value.sival_ptr = 0;
		ksev.sigev_signo = SIGCANCEL;
		ksev.sigev_notify = 4; /* SIGEV_THREAD_ID */
		ksev.sigev_tid = td->tid;
		r = syscall(SYS_timer_create, clk, &ksev, &timerid);
		pthread_barrier_wait(&args.b);
		if (r < 0) {
			pthread_cancel(td);
			return -1;
		}
		td->result = (void *)timerid;
		*res = td;
		break;
	default:
		errno = EINVAL;
		return -1;
	}

	return 0;
}
