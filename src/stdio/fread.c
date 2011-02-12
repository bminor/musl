#include "stdio_impl.h"

#define MIN(a,b) ((a)<(b) ? (a) : (b))

size_t fread(void *destv, size_t size, size_t nmemb, FILE *f)
{
	unsigned char *dest = destv;
	size_t len = size*nmemb, l = len, k;

	/* Never touch the file if length is zero.. */
	if (!l) return 0;

	FLOCK(f);

	for (;;) {
		/* First exhaust the buffer. */
		k = MIN(f->rend - f->rpos, l);
		memcpy(dest, f->rpos, k);
		f->rpos += k;
		dest += k;
		l -= k;

		/* Stop on EOF or errors */
		if (f->flags & (F_EOF|F_ERR|F_NORD)) goto eof;

		/* Done? Or going unbuffered? */
		if (!l || l > f->buf_size/2) break;

		/* Otherwise, refill & read thru buffer. */
		__underflow(f);
	}

	/* Read the remainder directly */
	for (; l; l-=k, dest+=k) {
		k = f->read(f, dest, l);
		if (k+1<=1) {
			f->flags |= F_EOF | (F_ERR & k);
			goto eof;
		}
	}

	FUNLOCK(f);
	return nmemb;
eof:
	FUNLOCK(f);
	return (len-l)/size;
}

weak_alias(fread, fread_unlocked);
