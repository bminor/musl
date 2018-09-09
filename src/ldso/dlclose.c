#include <dlfcn.h>
#include "dynlink.h"
#include "libc.h"

int dlclose(void *p)
{
	return __dl_invalid_handle(p);
}
