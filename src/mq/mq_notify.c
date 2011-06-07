#include <mqueue.h>
#include <pthread.h>
#include <errno.h>
#include "syscall.h"

int mq_notify(mqd_t mqd, const struct sigevent *sev)
{
	errno = ENOSYS;
	return -1;
}
