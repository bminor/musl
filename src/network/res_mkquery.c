#include <resolv.h>
#include <string.h>
#include <time.h>
#include "libc.h"

int __res_mkquery(int op, const char *dname, int class, int type,
	const unsigned char *data, int datalen,
	const unsigned char *newrr, unsigned char *buf, int buflen)
{
	int id, i, j;
	unsigned char q[280];
	struct timespec ts;
	size_t l = strnlen(dname, 254);

	if (l-1>=253 || buflen<18+l || op>15u || class>255u || type>255u)
		return -1;

	/* Construct query template - ID will be filled later */
	memset(q, 0, 18+l);
	q[2] = op*8 + 1;
	q[5] = 1;
	memcpy((char *)q+13, dname, l);
	for (i=13; q[i]; i=j+1) {
		for (j=i; q[j] && q[j] != '.'; j++);
		if (j-i-1u > 62u) return -1;
		q[i-1] = j-i;
	}
	q[i+1] = type;
	q[i+3] = class;

	/* Make a reasonably unpredictable id */
	clock_gettime(CLOCK_REALTIME, &ts);
	id = ts.tv_nsec + ts.tv_nsec/65536UL & 0xffff;
	q[0] = id/256;
	q[1] = id;

	memcpy(buf, q, 18+l);
	return 18+l;
}

weak_alias(__res_mkquery, res_mkquery);
