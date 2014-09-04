#include <aio.h>
#include <errno.h>
#include "libc.h"

int aio_fsync(int op, struct aiocb *cb)
{
	/* FIXME: unsupported */
	errno = EINVAL;
	return -1;
}

LFS64(aio_fsync);
