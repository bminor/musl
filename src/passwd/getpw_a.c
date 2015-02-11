#include "pwf.h"
#include <pthread.h>

int __getpw_a(const char *name, uid_t uid, struct passwd *pw, char **buf, size_t *size, struct passwd **res)
{
	FILE *f;
	int cs;
	int rv = 0;

	*res = 0;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	f = fopen("/etc/passwd", "rbe");
	if (!f) {
		rv = errno;
		goto done;
	}

	while (!(rv = __getpwent_a(f, pw, buf, size, res)) && *res) {
		if (name && !strcmp(name, (*res)->pw_name)
		|| !name && (*res)->pw_uid == uid)
			break;
	}
	fclose(f);

done:
	pthread_setcancelstate(cs, 0);
	if (rv) errno = rv;
	return rv;
}
