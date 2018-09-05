#include <dlfcn.h>
#include "libc.h"

hidden int __dl_invalid_handle(void *);

int dlclose(void *p)
{
	return __dl_invalid_handle(p);
}
