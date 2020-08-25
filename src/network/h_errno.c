#include <netdb.h>
#include "pthread_impl.h"

int *__h_errno_location(void)
{
	return &__pthread_self()->h_errno_val;
}
