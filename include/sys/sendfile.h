#ifndef _SYS_INOTIFY_H
#define _SYS_INOTIFY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

ssize_t sendfile(int, int, off_t *, size_t);

#ifdef __cplusplus
}
#endif

#endif
