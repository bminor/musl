#define _GNU_SOURCE
#include <dlfcn.h>

int __dladdr(void *, Dl_info *);

int dladdr(void *addr, Dl_info *info)
{
	return __dladdr(addr, info);
}
