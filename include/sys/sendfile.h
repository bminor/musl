#ifndef _SYS_INOTIFY_H
#define _SYS_INOTIFY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

ssize_t sendfile(int, int, off_t *, size_t);

#ifdef _LARGEFILE64_SOURCE
#define sendfile64 sendfile
#define off64_t off_t
#endif

#ifdef __cplusplus
}
#endif

#endif
