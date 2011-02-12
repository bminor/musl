#include "stdio_impl.h"

#define MIN(a,b) ((a)<(b) ? (a) : (b))

ssize_t getdelim(char **s, size_t *n, int delim, FILE *f)
{
	char *tmp;
	unsigned char *z;
	size_t k;
	size_t i=0;

	if (!n || !s) {
		errno = EINVAL;
		return -1;
	}

	if (!*s) *n=0;

	FLOCK(f);

	while (!feof(f)) {
		z = memchr(f->rpos, delim, f->rend - f->rpos);
		k = z ? z - f->rpos + 1 : f->rend - f->rpos;
		if (i+k >= *n) {
			if (k >= SIZE_MAX-i) goto oom;
			*n = i+k+1;
			if (*n < SIZE_MAX/2) *n *= 2;
			tmp = realloc(*s, *n);
			if (!tmp) {
				*n = i+k+1;
				tmp = realloc(*s, *n);
				if (!tmp) goto oom;
			}
			*s = tmp;
		}
		memcpy(*s+i, f->rpos, k);
		f->rpos += k;
		i += k;
		if (z) break;
		__underflow(f);
	}
	(*s)[i] = 0;
	if (feof(f) || ferror(f)) {
		FUNLOCK(f);
		return -1;
	}

	FUNLOCK(f);

	if (i > SSIZE_MAX) {
		errno = EOVERFLOW;
		return -1;
	}

	return i;
oom:
	errno = ENOMEM;
	return -1;
}
