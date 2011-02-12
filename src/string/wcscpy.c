#include <wchar.h>

wchar_t *wcscpy(wchar_t *d, const wchar_t *s)
{
	wchar_t *a = d;
	while ((*d++ = *s++));
	return a;
}
