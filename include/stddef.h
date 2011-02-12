#ifndef _STDDEF_H
#define _STDDEF_H

#undef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif

#define __NEED_ptrdiff_t
#define __NEED_size_t
#define __NEED_wchar_t

#include <bits/alltypes.h>

#define offsetof(type, member) ((size_t)( (char *)&(((type *)0)->member) - (char *)0 ))

#endif
