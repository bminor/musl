#ifndef _MONETARY_H
#define _MONETARY_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_ssize_t
#define __NEED_size_t
#define __NEED_locale_t

#include <bits/alltypes.h>

ssize_t strfmon(char *, size_t, const char *, ...);
ssize_t strfmon_l(char *, size_t, locale_t, const char *, ...);

#ifdef __cplusplus
}
#endif

#endif
