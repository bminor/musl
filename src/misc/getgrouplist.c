#include <grp.h>

/* FIXME */

int getgrouplist(const char *user, gid_t gid, gid_t *groups, int *ngroups)
{
	if (*ngroups<1) return -1;
	*groups = gid;
	*ngroups = 1;
	return 0;
}
