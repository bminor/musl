#include <spawn.h>

int posix_spawnattr_setsigmask(posix_spawnattr_t *attr, const sigset_t *mask)
{
	attr->__mask = *mask;
	return 0;
}
