#ifndef	_SYS_MMAN_H
#define	_SYS_MMAN_H
#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_mode_t
#define __NEED_size_t
#define __NEED_off_t

#include <bits/alltypes.h>

#include <bits/mman.h>

void *mmap (void *, size_t, int, int, int, off_t);
int munmap (void *, size_t);

int mprotect (void *, size_t, int);
int msync (void *, size_t, int);

int posix_madvise (void *, size_t, int);

int mlock (const void *, size_t);
int munlock (const void *, size_t);
int mlockall (int);
int munlockall (void);

/* linux extension */
void *mremap (void *, size_t, size_t, int, ...);

int shm_open (const char *, int, mode_t);
int shm_unlink (const char *);

#ifdef __cplusplus
}
#endif
#endif
