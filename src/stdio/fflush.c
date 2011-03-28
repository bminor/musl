#include "stdio_impl.h"

static int __fflush_unlocked(FILE *f)
{
	/* If writing, flush output */
	if (f->wpos > f->wbase) {
		f->write(f, 0, 0);
		if (!f->wpos) return EOF;
	}

	/* If reading, sync position, per POSIX */
	if (f->rpos < f->rend) f->seek(f, f->rpos-f->rend, SEEK_CUR);

	/* Clear read and write modes */
	f->wpos = f->wbase = f->wend = 0;
	f->rpos = f->rend = 0;

	/* Hook for special behavior on flush */
	if (f->flush) f->flush(f);

	return 0;
}

/* stdout.c will override this if linked */
static FILE *const __dummy = 0;
weak_alias(__dummy, __stdout_to_flush);

int fflush(FILE *f)
{
	int r;
	FILE *next;

	if (f) {
		FLOCK(f);
		r = __fflush_unlocked(f);
		FUNLOCK(f);
		return r;
	}

	r = __stdout_to_flush ? fflush(__stdout_to_flush) : 0;

	OFLLOCK();
	for (f=ofl_head; f; f=next) {
		FLOCK(f);
		//OFLUNLOCK();
		r |= __fflush_unlocked(f);
		//OFLLOCK();
		next = f->next;
		FUNLOCK(f);
	}
	OFLUNLOCK();
	
	return r;
}

weak_alias(__fflush_unlocked, fflush_unlocked);
