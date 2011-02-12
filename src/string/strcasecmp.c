#include <strings.h>
#include <ctype.h>

int strcasecmp(const char *_l, const char *_r)
{
	const unsigned char *l=_l, *r=_r;
	for (; *l && *r && (*l == *r || tolower(*l) == tolower(*r)); l++, r++);
	return tolower(*l) - tolower(*r);
}
