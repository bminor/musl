#include <mqueue.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include "syscall.h"

struct args {
	sem_t sem;
	int sock;
	const struct sigevent *sev;
};

static void *start(void *p)
{
	struct args *args = p;
	char buf[32];
	ssize_t n;
	int s = args->sock;
	void (*func)(union sigval) = args->sev->sigev_notify_function;
	union sigval val = args->sev->sigev_value;

	sem_post(&args->sem);
	n = recv(s, buf, sizeof(buf), MSG_NOSIGNAL|MSG_WAITALL);
	close(s);
	if (n==sizeof buf && buf[sizeof buf - 1] == 1)
		func(val);
	return 0;
}

int mq_notify(mqd_t mqd, const struct sigevent *sev)
{
	struct args args = { .sev = sev };
	pthread_attr_t attr;
	pthread_t td;
	int s;
	struct sigevent sev2;
	static const char zeros[32];
	int cs;

	if (!sev || sev->sigev_notify != SIGEV_THREAD)
		return syscall(SYS_mq_notify, mqd, sev);

	s = socket(AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, 0);
	if (s < 0) return -1;
	args.sock = s;

	if (sev->sigev_notify_attributes) attr = *sev->sigev_notify_attributes;
	else pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	sem_init(&args.sem, 0, 0);

	if (pthread_create(&td, &attr, start, &args)) {
		__syscall(SYS_close, s);
		errno = EAGAIN;
		return -1;
	}

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	sem_wait(&args.sem);
	pthread_setcancelstate(cs, 0);
	sem_destroy(&args.sem);

	sev2.sigev_notify = SIGEV_THREAD;
	sev2.sigev_signo = s;
	sev2.sigev_value.sival_ptr = (void *)&zeros;

	if (syscall(SYS_mq_notify, mqd, &sev2) < 0) {
		pthread_cancel(td);
		__syscall(SYS_close, s);
		return -1;
	}

	return 0;
}
