#include <stdlib.h>
#include <string.h>

/* A simple heap sort implementation.. only in-place O(nlogn) sort I know. */

#define MIN(a, b) ((a)<(b) ? (a) : (b))

static void swap(char *a, char *b, size_t len)
{
	char tmp[256];
	size_t l;
	while (len) {
		l = MIN(sizeof tmp, len);
		memcpy(tmp, a, l);
		memcpy(a, b, l);
		memcpy(b, tmp, l);
		a += l;
		b += l;
		len -= l;
	}
}

static void sift(char *base, size_t root, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	size_t max;

	while (2*root <= nel) {
		max = 2*root;
		if (max < nel && cmp(base+max*width, base+(max+1)*width) < 0)
			max++;
		if (cmp(base+root*width, base+max*width) < 0) {
			swap(base+root*width, base+max*width, width);
			root = max;
		} else break;
	}
}

void qsort(void *_base, size_t nel, size_t width, int (*cmp)(const void *, const void *))
{
	char *base = _base;
	size_t i;

	if (!nel) return;
	for (i=(nel+1)/2; i; i--)
		sift(base, i-1, nel-1, width, cmp);
	for (i=nel-1; i; i--) {
		swap(base, base+i*width, width);
		sift(base, 0, i-1, width, cmp);
	}
}
