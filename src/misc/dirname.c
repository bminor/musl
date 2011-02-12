#include <string.h>
#include <libgen.h>

char *dirname(char *s)
{
	size_t i;
	if (!s || !*s || !strchr(s, '/')) return ".";
	i = strlen(s)-1;
	for (; i&&s[i]=='/'; i--);
	for (; i&&s[i-1]!='/'; i--);
	for (; i&&s[i-1]=='/'; i--);
	if (!i && *s=='/') i++;
	s[i] = 0;
	return s;
}
