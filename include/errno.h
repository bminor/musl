#ifndef	_ERRNO_H
#define _ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bits/errno.h>

extern int *__errno_location(void);
#define errno (*__errno_location())

#ifdef __cplusplus
}
#endif

#endif

