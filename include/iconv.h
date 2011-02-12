#ifndef _ICONV_H
#define _ICONV_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size_t

#include <bits/alltypes.h>

typedef void *iconv_t;

iconv_t iconv_open(const char *, const char *);
size_t iconv(iconv_t, char **, size_t *, char **, size_t *);
int iconv_close(iconv_t);

#ifdef __cplusplus
}
#endif

#endif
