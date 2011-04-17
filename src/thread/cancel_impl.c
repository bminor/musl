#include "pthread_impl.h"

long __syscall_cp_asm(volatile void *, long, long, long, long, long, long, long);

long (__syscall_cp)(long nr, long u, long v, long w, long x, long y, long z)
{
	pthread_t self;
	uintptr_t old_sp, old_ip;
	long r;

	if (!libc.lock || (self = __pthread_self())->canceldisable)
		return __syscall(nr, u, v, w, x, y, z);

	old_sp = self->cp_sp;
	old_ip = self->cp_ip;
	self->cp_sp = 0;
	self->cp_ip = 0;
	r = __syscall_cp_asm(&self->cp_sp, nr, u, v, w, x, y, z);
	self->cp_sp = old_sp;
	self->cp_ip = old_ip;
	if (r == -EINTR && self->cancel) pthread_exit(PTHREAD_CANCELED);
	return r;
}

static void cancel_handler(int sig, siginfo_t *si, void *ctx)
{
	pthread_t self = __pthread_self();
	ucontext_t *uc = ctx;
	uintptr_t sp = ((uintptr_t *)&uc->uc_mcontext)[CANCEL_REG_SP];
	uintptr_t ip = ((uintptr_t *)&uc->uc_mcontext)[CANCEL_REG_IP];

	if (!self->cancel || self->canceldisable) return;

	if (self->cancelasync) pthread_exit(PTHREAD_CANCELED);

	if (sp != self->cp_sp) {
		if (!sp) return;
		sigaddset(&uc->uc_sigmask, SIGCANCEL);
		__syscall(SYS_tgkill, self->pid, self->tid, SIGCANCEL);
		return;
	}

	if (ip <= self->cp_ip) pthread_exit(PTHREAD_CANCELED);
}

static void testcancel()
{
	pthread_t self = __pthread_self();
	if (self->cancel && !self->canceldisable)
		pthread_exit(PTHREAD_CANCELED);
}

static void init_cancellation()
{
	struct sigaction sa = {
		.sa_flags = SA_SIGINFO | SA_RESTART,
		.sa_sigaction = cancel_handler
	};
	sigfillset(&sa.sa_mask);
	__libc_sigaction(SIGCANCEL, &sa, 0);
	libc.testcancel = testcancel;
}

int pthread_cancel(pthread_t t)
{
	static pthread_once_t once;
	pthread_once(&once, init_cancellation);
	a_store(&t->cancel, 1);
	return pthread_kill(t, SIGCANCEL);
}
