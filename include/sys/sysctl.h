#ifndef	_SYS_SYSCTL_H
#define	_SYS_SYSCTL_H

#define __NEED_size_t
#include <bits/alltypes.h>

int sysctl (int *, int, void *, size_t *, void *, size_t);

#endif
