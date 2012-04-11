#include "shgetc.h"

void __shlim(FILE *f, off_t lim)
{
	f->shlim = lim;
	f->shcnt = f->rend - f->rpos;
	if (lim && f->rend - f->rpos > lim)
		f->shend = f->rpos + lim;
	else
		f->shend = f->rend;
}

int __shgetc(FILE *f)
{
	int c;
	if (f->shcnt >= f->shlim) return EOF;
	c = __uflow(f);
	if (f->shlim && f->rend - f->rpos > f->shlim - f->shcnt - 1)
		f->shend = f->rpos + (f->shlim - f->shcnt - 1);
	else
		f->shend = f->rend;
	if (f->rend) f->shcnt += f->rend - f->buf;
	return c;
}
