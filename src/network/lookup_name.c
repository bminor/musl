#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "lookup.h"
#include "stdio_impl.h"
#include "syscall.h"

static int is_valid_hostname(const char *host)
{
	const unsigned char *s;
	if (strnlen(host, 255)-1 >= 254 || mbstowcs(0, host, 0) == -1) return 0;
	for (s=(void *)host; *s>=0x80 || *s=='.' || *s=='-' || isalnum(*s); s++);
	return !*s;
}

static int name_from_null(struct address buf[static 2], const char *name, int family, int flags)
{
	int cnt = 0;
	if (name) return 0;
	if (flags & AI_PASSIVE) {
		if (family != AF_INET6)
			buf[cnt++] = (struct address){ .family = AF_INET };
		if (family != AF_INET)
			buf[cnt++] = (struct address){ .family = AF_INET6 };
	} else {
		if (family != AF_INET6)
			buf[cnt++] = (struct address){ .family = AF_INET, .addr = { 127,0,0,1 } };
		if (family != AF_INET)
			buf[cnt++] = (struct address){ .family = AF_INET6, .addr = { [15] = 1 } };
	}
	return cnt;
}

static int name_from_numeric(struct address buf[static 1], const char *name, int family)
{
	return __lookup_ipliteral(buf, name, family);
}

static int name_from_hosts(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family)
{
	char line[512];
	size_t l = strlen(name);
	int cnt = 0;
	unsigned char _buf[1032];
	FILE _f, *f = __fopen_rb_ca("/etc/hosts", &_f, _buf, sizeof _buf);
	if (!f) return 0;
	while (fgets(line, sizeof line, f) && cnt < MAXADDRS) {
		char *p, *z;

		if ((p=strchr(line, '#'))) *p++='\n', *p=0;
		for(p=line+1; (p=strstr(p, name)) &&
			(!isspace(p[-1]) || !isspace(p[l])); p++);
		if (!p) continue;

		/* Isolate IP address to parse */
		for (p=line; *p && !isspace(*p); p++);
		*p++ = 0;
		if (name_from_numeric(buf+cnt, line, family))
			cnt++;

		/* Extract first name as canonical name */
		for (; *p && isspace(*p); p++);
		for (z=p; *z && !isspace(*z); z++);
		*z = 0;
		if (is_valid_hostname(p)) memcpy(canon, p, z-p+1);
	}
	__fclose_ca(f);
	return cnt;
}

struct dpc_ctx {
	struct address *addrs;
	char *canon;
	int cnt;
};

int __dns_parse(const unsigned char *, int, int (*)(void *, int, const void *, int, const void *), void *);
int __dn_expand(const unsigned char *, const unsigned char *, const unsigned char *, char *, int);
int __res_mkquery(int, const char *, int, int, const unsigned char *, int, const unsigned char*, unsigned char *, int);
int __res_msend(int, const unsigned char *const *, const int *, unsigned char *const *, int *, int);

#define RR_A 1
#define RR_CNAME 5
#define RR_AAAA 28

static int dns_parse_callback(void *c, int rr, const void *data, int len, const void *packet)
{
	char tmp[256];
	struct dpc_ctx *ctx = c;
	switch (rr) {
	case RR_A:
		if (len != 4) return -1;
		ctx->addrs[ctx->cnt].family = AF_INET;
		ctx->addrs[ctx->cnt].scopeid = 0;
		memcpy(ctx->addrs[ctx->cnt++].addr, data, 4);
		break;
	case RR_AAAA:
		if (len != 16) return -1;
		ctx->addrs[ctx->cnt].family = AF_INET6;
		ctx->addrs[ctx->cnt].scopeid = 0;
		memcpy(ctx->addrs[ctx->cnt++].addr, data, 16);
		break;
	case RR_CNAME:
		if (__dn_expand(packet, (const unsigned char *)packet + 512,
		    data, tmp, sizeof tmp) > 0 && is_valid_hostname(tmp))
			strcpy(ctx->canon, tmp);
		break;
	}
	return 0;
}

static int name_from_dns(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family)
{
	unsigned char qbuf[2][280], abuf[2][512];
	const unsigned char *qp[2] = { qbuf[0], qbuf[1] };
	unsigned char *ap[2] = { abuf[0], abuf[1] };
	int qlens[2], alens[2];
	int i, nq = 0;
	struct dpc_ctx ctx = { .addrs = buf, .canon = canon };

	if (family != AF_INET6) {
		qlens[nq] = __res_mkquery(0, name, 1, RR_A, 0, 0, 0,
			qbuf[nq], sizeof *qbuf);
		nq++;
	}
	if (family != AF_INET) {
		qlens[nq] = __res_mkquery(0, name, 1, RR_AAAA, 0, 0, 0,
			qbuf[nq], sizeof *qbuf);
		nq++;
	}

	if (__res_msend(nq, qp, qlens, ap, alens, sizeof *abuf) < 0) return EAI_SYSTEM;

	for (i=0; i<nq; i++)
		__dns_parse(abuf[i], alens[i], dns_parse_callback, &ctx);

	if (ctx.cnt) return ctx.cnt;
	if (alens[0] < 4 || (abuf[0][3] & 15) == 2) return EAI_AGAIN;
	if ((abuf[0][3] & 15) == 3) return EAI_NONAME;
	return EAI_FAIL;
}

int __lookup_name(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family, int flags)
{
	int cnt = 0, i, j;

	*canon = 0;
	if (name) {
		/* reject empty name and check len so it fits into temp bufs */
		size_t l = strnlen(name, 255);
		if (l-1 >= 254)
			return EAI_NONAME;
		memcpy(canon, name, l+1);
	}

	/* Procedurally, a request for v6 addresses with the v4-mapped
	 * flag set is like a request for unspecified family, followed
	 * by filtering of the results. */
	if (flags & AI_V4MAPPED) {
		if (family == AF_INET6) family = AF_UNSPEC;
		else flags -= AI_V4MAPPED;
	}

	/* Try each backend until there's at least one result. */
	cnt = name_from_null(buf, name, family, flags);
	if (!cnt) cnt = name_from_numeric(buf, name, family);
	if (!cnt && !(flags & AI_NUMERICHOST)) {
		cnt = name_from_hosts(buf, canon, name, family);
		if (!cnt) cnt = name_from_dns(buf, canon, name, family);
	}
	if (cnt<=0) return cnt ? cnt : EAI_NONAME;

	/* Filter/transform results for v4-mapped lookup, if requested. */
	if (flags & AI_V4MAPPED) {
		if (!(flags & AI_ALL)) {
			/* If any v6 results exist, remove v4 results. */
			for (i=0; i<cnt && buf[i].family != AF_INET6; i++);
			if (i<cnt) {
				for (j=0; i<cnt; i++) {
					if (buf[i].family == AF_INET6)
						buf[j++] = buf[i];
				}
				cnt = i = j;
			}
		}
		/* Translate any remaining v4 results to v6 */
		for (i=0; i<cnt; i++) {
			if (buf[i].family != AF_INET) continue;
			memcpy(buf[i].addr+12, buf[i].addr, 4);
			memcpy(buf[i].addr, "\0\0\0\0\0\0\0\0\0\0\xff\xff", 12);
			buf[i].family = AF_INET6;
		}
	}

	return cnt;
}
