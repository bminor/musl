#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>

int getdomainname(char *name, size_t len)
{
	*name = 0;
	return 0;
}
