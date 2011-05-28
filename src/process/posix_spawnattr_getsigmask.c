#include <spawn.h>

int posix_spawnattr_getsigmask(const posix_spawnattr_t *attr, sigset_t *mask)
{
	*mask = attr->__mask;
	return 0;
}
