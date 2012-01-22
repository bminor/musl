#include "pthread_impl.h"

void __cancel()
{
	pthread_t self = __pthread_self();
	self->cp_sp = 0;
	self->canceldisable = 1;
	self->cancelasync = 0;
	pthread_exit(PTHREAD_CANCELED);
}

long __syscall_cp_asm(volatile void *, long, long, long, long, long, long, long);

long (__syscall_cp)(long nr, long u, long v, long w, long x, long y, long z)
{
	pthread_t self;
	uintptr_t old_sp, old_ip;
	long r;

	if (!libc.main_thread || (self = __pthread_self())->canceldisable)
		return __syscall(nr, u, v, w, x, y, z);

	old_sp = self->cp_sp;
	old_ip = self->cp_ip;
	self->cp_sp = 0;
	self->cp_ip = 0;
	r = __syscall_cp_asm(&self->cp_sp, nr, u, v, w, x, y, z);
	self->cp_ip = old_ip;
	self->cp_sp = old_sp;
	if (r==-EINTR && nr!=SYS_close && self->cancel && !self->canceldisable)
		__cancel();
	return r;
}

static void _sigaddset(sigset_t *set, int sig)
{
	unsigned s = sig-1;
	set->__bits[s/8/sizeof *set->__bits] |= 1UL<<(s&8*sizeof *set->__bits-1);
}

static void cancel_handler(int sig, siginfo_t *si, void *ctx)
{
	pthread_t self = __pthread_self();
	ucontext_t *uc = ctx;
	uintptr_t sp = ((uintptr_t *)&uc->uc_mcontext)[CANCEL_REG_SP];
	uintptr_t ip = ((uintptr_t *)&uc->uc_mcontext)[CANCEL_REG_IP];

	if (!self->cancel || self->canceldisable) return;

	_sigaddset(&uc->uc_sigmask, SIGCANCEL);

	if (self->cancelasync || sp == self->cp_sp && ip <= self->cp_ip) {
		self->canceldisable = 1;
		pthread_sigmask(SIG_SETMASK, &uc->uc_sigmask, 0);
		__cancel();
	}

	if (self->cp_sp)
		__syscall(SYS_tgkill, self->pid, self->tid, SIGCANCEL);
}

void __testcancel()
{
	pthread_t self = pthread_self();
	if (self->cancel && !self->canceldisable)
		__cancel();
}

static void init_cancellation()
{
	struct sigaction sa = {
		.sa_flags = SA_SIGINFO | SA_RESTART,
		.sa_sigaction = cancel_handler
	};
	sigfillset(&sa.sa_mask);
	__libc_sigaction(SIGCANCEL, &sa, 0);
}

int pthread_cancel(pthread_t t)
{
	static int init;
	if (!init) {
		init_cancellation();
		init = 1;
	}
	a_store(&t->cancel, 1);
	return pthread_kill(t, SIGCANCEL);
}
