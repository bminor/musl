#include "pthread_impl.h"
#include <stdint.h>

__attribute__((__visibility__("hidden")))
void *__aeabi_read_tp_c(void)
{
	return TP_ADJ(__pthread_self());
}
