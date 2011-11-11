#ifndef _SYS_UIO_H
#define _SYS_UIO_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_struct_iovec

#include <bits/alltypes.h>

ssize_t readv (int, const struct iovec *, int);
ssize_t writev (int, const struct iovec *, int);

#ifdef __cplusplus
}
#endif

#endif
