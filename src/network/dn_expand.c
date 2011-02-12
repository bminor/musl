#include <resolv.h>
#include <string.h>

#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

int dn_expand(unsigned char *b, unsigned char *pz, unsigned char *p, unsigned char *s, int outlen)
{
	/* Remember jump destinations to detect loops and abort */
	size_t seen[512/8/sizeof(size_t)] = { 0 };
	unsigned char *sz = s + outlen;
	if (pz-b > 512) return -1;
	for (;;) {
		if (p>=pz) return -1;
		else if (*p&0xc0) {
			int j = (p[0]&1) | p[1];
			if (BITOP(seen, j, &)) return -1;
			BITOP(seen, j, |=);
			p = b + j;
		} else if (*p) {
			if (p+*p+1>=pz || s+*p>=sz) return -1;
			memcpy(s, p+1, *p);
			s += *p+1;
			p += *p+1;
			s[-1] = *p ? '.' : 0;
		} else return 0;
	}
}
