#include <wchar.h>
#include <wctype.h>
#include <ctype.h>

int iswspace(wint_t wc)
{
	static const wchar_t spaces[] = {
		' ', '\t', '\n', '\r', 11, 12,  0x0085,
		0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005,
		0x2006, 0x2008, 0x2009, 0x200a, 0x200b,
		0x2028, 0x2029, 0x2050, 0x3000, 0
	};
	if (wcschr(spaces, wc)) return 1;
	return 0;
}
