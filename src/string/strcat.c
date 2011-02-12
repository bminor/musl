#include <string.h>

char *strcat(char *dest, const char *src)
{
	strcpy(dest + strlen(dest), src);
	return dest;
}
