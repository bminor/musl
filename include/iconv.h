#ifndef _ICONV_H
#define _ICONV_H

#ifdef __cplusplus
extern "C" {
#endif

#if __STDC_VERSION__ >= 199901L
#define __restrict restrict
#elif !defined(__GNUC__)
#define __restrict
#endif

#define __NEED_size_t

#include <bits/alltypes.h>

typedef void *iconv_t;

iconv_t iconv_open(const char *, const char *);
size_t iconv(iconv_t, char **__restrict, size_t *__restrict, char **__restrict, size_t *__restrict);
int iconv_close(iconv_t);

#ifdef __cplusplus
}
#endif

#endif
