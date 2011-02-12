#ifndef	_SYS_STATFS_H
#define	_SYS_STATFS_H

#include <sys/statvfs.h>

#define statfs statvfs
#define fstatfs fstatvfs
#define f_namelen f_namemax

#endif
