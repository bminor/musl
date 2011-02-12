#include "syscall.h"

int __set_thread_area(unsigned long *desc)
{
	if (syscall1(__NR_set_thread_area, (long)desc) < 0)
		return -1;
	__asm__ __volatile__ ( "movw %w0,%%gs" : : "r"(desc[0]*8+3) );
	return 0;
}
