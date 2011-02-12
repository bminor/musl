#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

/* This function should work with most dlmalloc-like chunk bookkeeping
 * systems, but it's only guaranteed to work with the native implementation
 * used in this library. */

int posix_memalign(void **res, size_t align, size_t len)
{
	unsigned char *mem, *new, *end;
	size_t header, footer;

	if ((align & -align) != align) return EINVAL;
	if (len > SIZE_MAX - align) return ENOMEM;

	if (align <= 4*sizeof(size_t)) {
		if (!(mem = malloc(len)))
			return errno;
		*res = mem;
		return 0;
	}

	if (!(mem = malloc(len + align-1)))
		return errno;

	header = ((size_t *)mem)[-1];
	end = mem + (header & -8);
	footer = ((size_t *)end)[-2];
	new = (void *)((uintptr_t)mem + align-1 & -align);

	if (!(header & 7)) {
		((size_t *)new)[-2] = ((size_t *)mem)[-2] + (new-mem);
		((size_t *)new)[-1] = ((size_t *)mem)[-1] - (new-mem);
		*res = new;
		return 0;
	}

	((size_t *)mem)[-1] = header&7 | new-mem;
	((size_t *)new)[-2] = footer&7 | new-mem;
	((size_t *)new)[-1] = header&7 | end-new;
	((size_t *)end)[-2] = footer&7 | end-new;

	if (new != mem) free(mem);
	*res = new;
	return 0;
}
