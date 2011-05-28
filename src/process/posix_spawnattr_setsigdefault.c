#include <spawn.h>

int posix_spawnattr_setsigdefault(posix_spawnattr_t *attr, const sigset_t *def)
{
	attr->__def = *def;
	return 0;
}
