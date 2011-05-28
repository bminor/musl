#include <spawn.h>

int posix_spawnattr_getpgroup(const posix_spawnattr_t *attr, pid_t *pgrp)
{
	*pgrp = attr->__pgrp;
	return 0;
}
