#include <wchar.h>

wchar_t *wcsncpy(wchar_t *d, const wchar_t *s, size_t n)
{
	wchar_t *a = d;
	while (n && *s) n--, *d++ = *s++;
	wmemset(d, 0, n);
	return a;
}
