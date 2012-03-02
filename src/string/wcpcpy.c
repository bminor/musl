#include <wchar.h>

wchar_t *wcpcpy(wchar_t *d, const wchar_t *s)
{
	return wcscpy(d, s) + wcslen(s);
}
