#include <spawn.h>

int posix_spawnattr_getsigdefault(const posix_spawnattr_t *attr, sigset_t *def)
{
	*def = attr->__def;
	return 0;
}
