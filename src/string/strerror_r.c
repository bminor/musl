#include <string.h>
#include <errno.h>

int strerror_r(int err, char *buf, size_t buflen)
{
	char *msg = strerror(err);
	if (strlen(msg) >= buflen)
		return ERANGE;
	strcpy(buf, msg);
	return 0;
}
