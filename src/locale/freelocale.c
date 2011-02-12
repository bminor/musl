#include <stdlib.h>
#include "locale_impl.h"

void freelocale(locale_t l)
{
	free(l);
}
