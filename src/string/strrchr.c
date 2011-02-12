#include <string.h>

char *strrchr(const char *s, int c)
{
	const char *p;
	c = (char)c;
	for (p=s+strlen(s); p>=s && *p!=c; p--);
	return p>=s ? (char *)p : 0;
}
