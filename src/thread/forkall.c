#if 0
#include "pthread_impl.h"
#include <setjmp.h>

struct thread {
	struct thread *next;
	pthread_t td;
	jmp_buf jb;
	void *tmp, *stack;
};

struct ctx {
	struct thread *list;
	pthread_t caller;
	pid_t pid;
	size_t cnt;
	pthread_barrier_t barrier;
};

static void restart_thread(pthread_t self)
{
	struct thread *t = self->start_arg;
	self->start_arg = t->tmp;
	self->pid = getpid();
	longjmp(t->jb, 1);
}

static void do_forkall(void *p)
{
	struct ctx *c = p, *volatile cv = c;
	char tmpstack[2048];
	struct thread *tp, t = {
		.td = __pthread_self(),
		.next = c->list,
		.stack = tmpstack+1024
	};

	if (t.td != c->caller) {
		c->cnt++;
		t.tmp = t.td->start_arg;
		t.td->start_arg = &t;
		if (setjmp(t.jb)) {
			c = cv;
			if (c->pid) return;
			pthread_barrier_wait(&c->barrier);
			return;
		}
		c->list = &t;
		__synccall_wait();
		return;
	}
	c->pid = syscall(SYS_fork);
	if (c->pid) return;

	pthread_barrier_init(&c->barrier, 0, c->cnt);
	for (tp=c->list; tp; tp=tp->next)
		if (__uniclone(tp->stack, restart_thread, tp->td) < 0)
			_exit(127);
	pthread_barrier_wait(&c->barrier);
}

pid_t forkall()
{
	struct ctx c = { .caller = pthread_self() };
	__synccall(do_forkall, &c);
	return c.pid;
}
#endif
