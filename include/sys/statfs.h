#ifndef	_SYS_STATFS_H
#define	_SYS_STATFS_H

#include <sys/statvfs.h>

typedef struct {
	int __val[2];
} fsid_t;

#include <bits/statfs.h>

int statfs (const char *, struct statfs *);
int fstatfs (int, struct statfs *);

#endif
