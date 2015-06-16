#include <stdio.h>
#include <wchar.h>

wint_t btowc(int c)
{
	c = (unsigned char)c;
	return c<128U ? c : EOF;
}
