#include <mqueue.h>
#include "syscall.h"

ssize_t mq_timedreceive(mqd_t mqd, char *msg, size_t len, unsigned *prio, const struct timespec *at)
{
	return syscall_cp(SYS_mq_timedreceive, mqd, msg, len, prio, at);
}
