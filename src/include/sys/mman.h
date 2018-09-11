#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include "../../../include/sys/mman.h"

void __vm_wait(void);
void __vm_lock(void);
void __vm_unlock(void);

void *__mmap(void *, size_t, int, int, int, off_t);
int __munmap(void *, size_t);
void *__mremap(void *, size_t, size_t, int, ...);
int __madvise(void *, size_t, int);
int __mprotect(void *, size_t, int);

const unsigned char *__map_file(const char *, size_t *);

char *__shm_mapname(const char *, char *);

#endif
