#include <aio.h>
#include "libc.h"

int aio_error(const struct aiocb *cb)
{
	return cb->__err;
}

LFS64(aio_error);
