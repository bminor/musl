#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

/* This function should work with most dlmalloc-like chunk bookkeeping
 * systems, but it's only guaranteed to work with the native implementation
 * used in this library. */

void *aligned_alloc(size_t align, size_t len)
{
	unsigned char *mem, *new, *end;
	size_t header, footer;

	if ((align & -align & -sizeof(void *)) != align) {
		errno = EINVAL;
		return NULL;
	}

	if (len > SIZE_MAX - align) {
		errno = ENOMEM;
		return NULL;
	}

	if (align <= 4*sizeof(size_t)) {
		if (!(mem = malloc(len)))
			return NULL;
		return mem;
	}

	if (!(mem = malloc(len + align-1)))
		return NULL;

	header = ((size_t *)mem)[-1];
	end = mem + (header & -8);
	footer = ((size_t *)end)[-2];
	new = (void *)((uintptr_t)mem + align-1 & -align);

	if (!(header & 7)) {
		((size_t *)new)[-2] = ((size_t *)mem)[-2] + (new-mem);
		((size_t *)new)[-1] = ((size_t *)mem)[-1] - (new-mem);
		return new;
	}

	((size_t *)mem)[-1] = header&7 | new-mem;
	((size_t *)new)[-2] = footer&7 | new-mem;
	((size_t *)new)[-1] = header&7 | end-new;
	((size_t *)end)[-2] = footer&7 | end-new;

	if (new != mem) free(mem);
	return new;
}
