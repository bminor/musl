#ifndef	_DIRENT_H
#define	_DIRENT_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_ino_t
#define __NEED_off_t

#include <bits/alltypes.h>

typedef struct __DIR_s DIR;

struct dirent
{
	ino_t d_ino;
	off_t d_off;
	unsigned short d_reclen;
	unsigned char d_type;
	char d_name[1];
};

int            closedir(DIR *);
DIR           *fdopendir(int);
DIR           *opendir(const char *);
struct dirent *readdir(DIR *);
int            readdir_r(DIR *, struct dirent *, struct dirent **);
void           rewinddir(DIR *);
void           seekdir(DIR *, long);
long           telldir(DIR *);
int            dirfd(DIR *);

int alphasort(const struct dirent **, const struct dirent **);
int scandir(const char *, struct dirent ***, int (*)(const struct dirent *), int (*)(const struct dirent **, const struct dirent **));

#ifdef __cplusplus
extern }
#endif

#endif
