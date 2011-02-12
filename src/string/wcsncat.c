#include <wchar.h>

wchar_t *wcsncat(wchar_t *d, const wchar_t *s, size_t n)
{
	wchar_t *a = d;
	d += wcslen(d);
	while (n && (*d++ = *s++)) n--;
	*d++ = 0;
	return a;
}
