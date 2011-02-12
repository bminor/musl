#include "stdio_impl.h"

int fclose(FILE *f)
{
	int r;
	int perm = f->flags & F_PERM;

	if (!perm) {
		OFLLOCK();
		if (f->prev) f->prev->next = f->next;
		if (f->next) f->next->prev = f->prev;
		if (ofl_head == f) ofl_head = f->next;
		OFLUNLOCK();
	}

	r = fflush(f) | f->close(f);

	if (!perm) free(f);
	
	return r;
}
