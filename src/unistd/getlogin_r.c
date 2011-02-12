#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int getlogin_r(char *name, size_t size)
{
	char *logname = getlogin();
	if (!logname) return ENXIO; /* or...? */
	if (strlen(name) >= size) return ERANGE;
	strcpy(name, logname);
	return 0;
}
