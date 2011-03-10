#include <signal.h>
#include <errno.h>
#include <pthread.h>

int pthread_sigmask(int how, const sigset_t *set, sigset_t *old)
{
	int ret = sigprocmask(how, set, old);
	if (ret) return errno;
	return 0;
}
