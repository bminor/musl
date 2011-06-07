#include <mqueue.h>
#include "syscall.h"

int mq_setattr(mqd_t mqd, const struct mq_attr *new, struct mq_attr *old)
{
	return syscall(SYS_mq_getsetattr, mqd, new, old);
}
