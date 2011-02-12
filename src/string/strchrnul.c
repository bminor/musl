#include <string.h>

char *strchrnul(const char *s, int c)
{
	char *p = strchr(s, c);
	return p ? p : (char *)s + strlen(s);
}
