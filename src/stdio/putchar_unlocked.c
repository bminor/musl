#include "stdio_impl.h"

int putchar_unlocked(int c)
{
	return stdout->wpos < stdout->wstop ?
		(*stdout->wpos++ = c) : __overflow(stdout, c);
}
