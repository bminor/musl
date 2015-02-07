#include <pthread.h>
#include "pwf.h"

int __getgr_a(const char *name, gid_t gid, struct group *gr, char **buf, size_t *size, char ***mem, size_t *nmem, struct group **res)
{
	FILE *f;
	int rv = 0;
	int cs;

	*res = 0;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

	f = fopen("/etc/group", "rbe");
	if (!f) {
		rv = errno;
		goto done;
	}

	while (!(rv = __getgrent_a(f, gr, buf, size, mem, nmem, res)) && *res) {
		if (name && !strcmp(name, (*res)->gr_name)
		|| !name && (*res)->gr_gid == gid) {
			break;
		}
	}
	fclose(f);

done:
	pthread_setcancelstate(cs, 0);
	if (rv) errno = rv;
	return rv;
}
