#include <ftw.h>
#include "libc.h"

int ftw(const char *path, int (*fn)(const char *, const struct stat *, int), int fd_limit)
{
	return nftw(path, (void *)fn, fd_limit, FTW_PHYS);
}

LFS64(ftw);
