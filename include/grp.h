#ifndef	_GRP_H
#define	_GRP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

#define __NEED_size_t
#define __NEED_gid_t

#include <bits/alltypes.h>

struct group
{
	char *gr_name;
	char *gr_passwd;
	gid_t gr_gid;
	char **gr_mem;
};

struct group  *getgrgid(gid_t);
struct group  *getgrnam(const char *);

int getgrgid_r(gid_t, struct group *, char *, size_t, struct group **);
int getgrnam_r(const char *, struct group *, char *, size_t, struct group **);

struct group  *getgrent(void);
void           endgrent(void);
void           setgrent(void);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int getgrouplist(const char *, gid_t, gid_t *, int *);
int setgroups(size_t, const gid_t *);
int initgroups(const char *, gid_t);
#endif

#ifdef __cplusplus
}
#endif

#endif
