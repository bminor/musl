#include "stdio_impl.h"

#define MIN(a,b) ((a)<(b) ? (a) : (b))

char *fgets(char *s, int n, FILE *f)
{
	char *p = s;
	unsigned char *z;
	size_t k;

	if (!n--) return 0;

	FLOCK(f);

	while (n && !feof(f)) {
		z = memchr(f->rpos, '\n', f->rend - f->rpos);
		k = z ? z - f->rpos + 1 : f->rend - f->rpos;
		k = MIN(k, n);
		memcpy(p, f->rpos, k);
		f->rpos += k;
		p += k;
		n -= k;
		if (z) break;
		__underflow(f);
	}
	*p = 0;
	if (ferror(f)) p = s;

	FUNLOCK(f);

	return (p == s) ? 0 : s;
}

weak_alias(fgets, fgets_unlocked);
