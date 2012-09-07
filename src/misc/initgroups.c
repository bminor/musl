#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <limits.h>

int getgrouplist(const char *, gid_t, gid_t *, int *);
int setgroups(size_t, const gid_t *);

int initgroups(const char *user, gid_t gid)
{
	gid_t groups[NGROUPS_MAX];
	int count = NGROUPS_MAX;
	if (getgrouplist(user, gid, groups, &count) < 0) return -1;
	return setgroups(count, groups);
}
