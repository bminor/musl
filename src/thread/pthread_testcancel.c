#include "pthread_impl.h"
#include "libc.h"

static void dummy()
{
}

weak_alias(dummy, __testcancel);

void pthread_testcancel()
{
	__testcancel();
}
