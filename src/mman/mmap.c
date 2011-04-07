#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include "syscall.h"
#include "libc.h"

void *__mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
	if (sizeof(off_t) > sizeof(long))
		if (((long)off & 0xfff) | ((long)((unsigned long long)off>>(12 + 8*(sizeof(off_t)-sizeof(long))))))
			start = (void *)-1;
#ifdef SYS_mmap2
	return (void *)syscall(SYS_mmap2, start, len, prot, flags, fd, off>>12);
#else
	return (void *)syscall(SYS_mmap, start, len, prot, flags, fd, off);
#endif
}

weak_alias(__mmap, mmap);

LFS64(mmap);
