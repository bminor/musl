#include "stdio_impl.h"

size_t __fwritex(const unsigned char *s, size_t l, FILE *f)
{
	size_t i = 0;
	size_t k = f->wend - f->wpos;

	/* Handle line-buffered mode by breaking into 2 parts */
	if (f->lbf >= 0) {
		/* Match /^(.*\n|)/ */
		for (i=l; i && s[i-1] != '\n'; i--);
		if (i) {
			f->lbf = EOF;
			__fwritex(s, i, f);
			f->lbf = '\n';
			__oflow(f);
			return ferror(f) ? 0 : i + __fwritex(s+i, l-i, f);
		}
	}

	/* Buffer initial segment */
	if (k > l) k = l;
	memcpy(f->wpos, s, k);
	f->wpos += k;
	if (f->wpos < f->wend) return l;

	/* If there's work left to do, flush buffer */
	__oflow(f);
	if (ferror(f)) return 0;

	/* If the remainder will not fit in buffer, write it directly */
	if (l - k >= f->wend - f->wpos)
		return k + f->write(f, s+k, l-k);

	/* Otherwise, buffer the remainder */
	memcpy(f->wpos, s+k, l-k);
	f->wpos += l-k;
	return l;
}

size_t fwrite(const void *src, size_t size, size_t nmemb, FILE *f)
{
	size_t l = size*nmemb;
	if (!l) return l;
	FLOCK(f);
	l = __fwritex(src, l, f);
	FUNLOCK(f);
	return l/size;
}

weak_alias(fwrite, fwrite_unlocked);
