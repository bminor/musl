#include <aio.h>
#include "libc.h"

ssize_t aio_return(struct aiocb *cb)
{
	return cb->__ret;
}

LFS64(aio_return);
