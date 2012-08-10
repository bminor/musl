#include "pthread_impl.h"
#include <semaphore.h>

static struct chain {
	struct chain *next;
	sem_t sem, sem2;
} *head, *cur;

static void (*callback)(void *), *context;
static int chainlen;
static sem_t chainlock, chaindone;
static pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

static void handler(int sig, siginfo_t *si, void *ctx)
{
	struct chain ch;
	pthread_t self = __pthread_self();
	int old_errno = errno;

	if (chainlen == libc.threads_minus_1) return;

	sigqueue(self->pid, SIGSYNCCALL, (union sigval){0});

	/* Threads which have already decremented themselves from the
	 * thread count must not act. Block further receipt of signals
	 * and return. */
	if (self->dead) {
		memset(&((ucontext_t *)ctx)->uc_sigmask, -1, 8);
		errno = old_errno;
		return;
	}

	sem_init(&ch.sem, 0, 0);
	sem_init(&ch.sem2, 0, 0);

	while (sem_wait(&chainlock));
	ch.next = head;
	head = &ch;
	if (++chainlen == libc.threads_minus_1) sem_post(&chaindone);
	sem_post(&chainlock);

	while (sem_wait(&ch.sem));
	callback(context);
	sem_post(&ch.sem2);
	while (sem_wait(&ch.sem));

	errno = old_errno;
}

void __synccall(void (*func)(void *), void *ctx)
{
	pthread_t self;
	struct sigaction sa;
	struct chain *next;
	uint64_t oldmask;

	if (!libc.threads_minus_1) {
		func(ctx);
		return;
	}

	pthread_rwlock_wrlock(&lock);

	__syscall(SYS_rt_sigprocmask, SIG_BLOCK, SIGALL_SET,
		&oldmask, __SYSCALL_SSLEN);

	sem_init(&chaindone, 0, 0);
	sem_init(&chainlock, 0, 1);
	chainlen = 0;
	callback = func;
	context = ctx;

	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sa.sa_sigaction = handler;
	sigfillset(&sa.sa_mask);
	__libc_sigaction(SIGSYNCCALL, &sa, 0);

	self = __pthread_self();
	sigqueue(self->pid, SIGSYNCCALL, (union sigval){0});
	while (sem_wait(&chaindone));

	for (cur=head; cur; cur=cur->next) {
		sem_post(&cur->sem);
		while (sem_wait(&cur->sem2));
	}
	func(ctx);

	for (cur=head; cur; cur=next) {
		next = cur->next;
		sem_post(&cur->sem);
	}

	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	__libc_sigaction(SIGSYNCCALL, &sa, 0);

	__syscall(SYS_rt_sigprocmask, SIG_SETMASK,
		&oldmask, 0, __SYSCALL_SSLEN);

	pthread_rwlock_unlock(&lock);
}

void __synccall_lock()
{
	pthread_rwlock_rdlock(&lock);
}

void __synccall_unlock()
{
	pthread_rwlock_unlock(&lock);
}
