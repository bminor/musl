#include "stdio_impl.h"

void __shlim(FILE *, off_t);
int __shgetc(FILE *);

static inline off_t shcnt(FILE *f)
{
	return f->shcnt + (f->rpos - f->rend);
}

static inline void shlim(FILE *f, off_t lim)
{
	__shlim(f, lim);
}

static inline int shgetc(FILE *f)
{
	if (f->rpos < f->shend) return *f->rpos++;
	return __shgetc(f);
}

static inline void shunget(FILE *f)
{
	if (f->rend) f->rpos--;
}
