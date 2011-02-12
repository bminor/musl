#include <wctype.h>

int iswprint(wint_t wc)
{
	unsigned c = wc;
	/* assume any non-control, non-illegal codepoint is printable */
	if (c>0x10ffff || c-0xd800<0x800 || (c&0xfffe)==0xfffe || iswcntrl(c))
		return 0;
	return 1;
}
