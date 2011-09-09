#include <aio.h>

int aio_error(struct aiocb *cb)
{
	return cb->__err;
}
