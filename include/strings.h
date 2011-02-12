#ifndef	_STRINGS_H
#define	_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#undef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif


#define __NEED_size_t
#include <bits/alltypes.h>


int bcmp (const void *, const void *, size_t);
void bcopy (const void *, void *, size_t);
void bzero (void *, size_t);

int ffs (int);

char *index (const char *, int);
char *rindex (const char *, int);

int strcasecmp (const char *, const char *);
int strncasecmp (const char *, const char *, size_t);

#ifdef __cplusplus
}
#endif

#endif
