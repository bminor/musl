#include <spawn.h>

int posix_spawnattr_getflags(const posix_spawnattr_t *attr, short *flags)
{
	*flags = attr->__flags;
	return 0;
}
