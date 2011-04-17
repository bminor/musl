#include "pthread_impl.h"

void pthread_testcancel()
{
	if (libc.testcancel) libc.testcancel();
}
