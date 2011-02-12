#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include "libc.h"

uintptr_t __brk(uintptr_t);

#define ALIGN 16

void *__simple_malloc(size_t n)
{
	static uintptr_t cur, brk;
	uintptr_t base, new;
	static int lock;
	size_t align=1;

	if (n < SIZE_MAX - ALIGN)
		while (align<n && align<ALIGN)
			align += align;
	n = n + align - 1 & -align;

	LOCK(&lock);
	if (!cur) cur = brk = __brk(0)+16;
	if (n > SIZE_MAX - brk) goto fail;

	base = cur + align-1 & -align;
	if (base+n > brk) {
		new = base+n + PAGE_SIZE-1 & -PAGE_SIZE;
		if (__brk(new) != new) goto fail;
		brk = new;
	}
	cur = base+n;
	UNLOCK(&lock);

	return (void *)base;

fail:
	UNLOCK(&lock);
	errno = ENOMEM;
	return 0;
}

weak_alias(__simple_malloc, malloc);
