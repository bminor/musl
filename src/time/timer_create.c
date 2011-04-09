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

static void dummy_1(pthread_t self)
{
}
weak_alias(dummy_1, __pthread_tsd_run_dtors);

static void cleanup_fromsig(void *p)
{
	pthread_t self = __pthread_self();
	self->cancel = 0;
	self->cancelbuf = 0;
	__pthread_tsd_run_dtors(self);
	longjmp(p, 1);
}

void __sigtimer_handler(pthread_t self)
{
	jmp_buf jb;
	void (*notify)(union sigval) = (void (*)(union sigval))self->start;
	union sigval val = { .sival_ptr = self->start_arg };

	if (setjmp(jb)) return;
	pthread_cleanup_push(cleanup_fromsig, jb);
	notify(val);
	pthread_cleanup_pop(0);
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

	pthread_barrier_wait(&args->b);
	__wait(&self->delete_timer, 0, 0, 1);
	__syscall(SYS_timer_delete, self->result);
	return 0;
}

int timer_create(clockid_t clk, struct sigevent *evp, timer_t *res)
{
	pthread_t td;
	pthread_attr_t attr;
	int r;
	struct start_args args;
	struct ksigevent ksev, *ksevp=0;
	int timerid;

	switch (evp ? evp->sigev_notify : SIGEV_SIGNAL) {
	case SIGEV_NONE:
	case SIGEV_SIGNAL:
		if (evp) {
			ksev.sigev_value = evp->sigev_value;
			ksev.sigev_signo = evp->sigev_signo;
			ksev.sigev_notify = evp->sigev_notify;
			ksev.sigev_tid = 0;
			ksevp = &ksev;
		}
		if (syscall(SYS_timer_create, clk, ksevp, &timerid) < 0)
			return -1;
		*res = (void *)timerid;
		break;
	case SIGEV_THREAD:
		if (evp->sigev_notify_attributes)
			attr = *evp->sigev_notify_attributes;
		else
			pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_barrier_init(&args.b, 0, 2);
		args.sev = evp;
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
