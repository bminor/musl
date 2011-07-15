#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include "intparse.h"

/* Lookup table for digit values. -1==255>=36 -> invalid */
static const unsigned char digits[] = {
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,-1,-1,-1,-1,-1,-1,
-1,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
25,26,27,28,29,30,31,32,33,34,35,-1,-1,-1,-1,-1,
-1,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
25,26,27,28,29,30,31,32,33,34,35,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

#define SLIM (UINT_MAX/36-1)
#define LLIM (UINTMAX_MAX/36-1)

int __intparse(struct intparse *v, const void *buf, size_t n)
{
	const unsigned char *s = buf;
	int d, b = v->base;

	v->cnt += n;
	for (; n; n--, s++) switch (v->state) {
	case 0:
		v->err = EINVAL;
		v->state++;
		if (*s=='+' || *s=='-') {
			v->neg = *s=='-';
			continue;
		}
	case 1:
		v->state++;
		if (*s=='0' && (!b || b==16)) continue;
		if (!b) v->base = b = 10;
		v->state++;
		goto firstdigit;
	case 2:
		v->state++;
		if ((!b || b==16) && (*s|32) == 'x') {
			v->err = 0;
			v->base = b = 16;
			continue;
		}
		if (!b) v->base = b = 8;
		goto seconddigit;
	case 3:
	firstdigit:
		if (digits[*s] >= b) {
			n++;
			goto finished;
		}
	seconddigit:
		v->err = 0;
		v->state++;
	case 4:
		if (b==10) {
			for (; n && *s-'0'<10U && v->small<=SLIM; n--, s++)
				v->small = v->small * 10 + (*s-'0');
		} else if ((b&-b) == b) {
			int bs = "\0\1\2\4\7\3\6\5"[(0x17*b)>>5&7];
			for (; n && (d=digits[*s])<b && v->small<=SLIM; n--, s++)
				v->small = (v->small<<bs) + d;
		} else {
			for (; n && (d=digits[*s])<b && v->small<=SLIM; n--, s++)
				v->small = v->small * b + d;
		}
		if (!n) return 1;
		v->state++;
		v->val = v->small;
	case 5:
		for (; n && (d=digits[*s])<b && v->val<=LLIM; n--, s++)
			v->val = v->val * b + d;
		if (!n) return 1;
		if (d >= b) goto finished;
		if (v->val < (UINTMAX_MAX-d)/b)
			v->val = v->val * b + d;
		else
			v->err = ERANGE;
		v->state++;
		n--; s++;
	case 6:
		if (n && digits[*s]<b) {
			v->err = ERANGE;
			v->val = UINTMAX_MAX;
			n--; s++;
			for (; n && digits[*s]<b; n--, s++);
		}
		if (!n) return 1;
		goto finished;
	}
	return 1;
finished:
	v->cnt -= n;
	return 0;
}
