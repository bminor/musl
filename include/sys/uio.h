#ifndef _SYS_UIO_H
#define _SYS_UIO_H

#define __NEED_size_t
#define __NEED_ssize_t

#include <bits/alltypes.h>

#include <bits/uio.h>

ssize_t readv (int, const struct iovec *, int);
ssize_t writev (int, const struct iovec *, int);

#endif
