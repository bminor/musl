#include "libc.h"

hidden int __shcall(void *arg, int (*func)(void *))
{
	return func(arg);
}
