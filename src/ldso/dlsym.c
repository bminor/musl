#include <dlfcn.h>

void *__dlsym(void *, const char *, void *);

void *dlsym(void *p, const char *s)
{
	return __dlsym(p, s, 0);
}
