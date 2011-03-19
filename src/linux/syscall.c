#include "syscall.h"
#include <stdarg.h>

#undef syscall

long syscall(long n, ...)
{
	va_list ap;
	long a,b,c,d,e,f;
	va_start(ap, n);
	a=va_arg(ap, long);
	b=va_arg(ap, long);
	c=va_arg(ap, long);
	d=va_arg(ap, long);
	e=va_arg(ap, long);
	f=va_arg(ap, long);
	va_end(ap);
	return __syscall_ret(__syscall(n,a,b,c,d,e,f));
}
