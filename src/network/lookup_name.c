#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "lookup.h"
#include "stdio_impl.h"
#include "syscall.h"
#include "__dns.h"

static int is_valid_hostname(const char *host)
{
	const unsigned char *s;
	if (strnlen(host, 256)-1 > 254 || mbstowcs(0, host, 0) > 255) return 0;
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
	struct in_addr a4;
	struct in6_addr a6;
	if (family != AF_INET6 && inet_aton(name, &a4)>0) {
		memcpy(&buf[0].addr, &a4, sizeof a4);
		buf[0].family = AF_INET;
		return 1;
	}
	if (family != AF_INET && inet_pton(AF_INET6, name, &a6)>0) {
		memcpy(&buf[0].addr, &a6, sizeof a6);
		buf[0].family = AF_INET6;
		return 1;
	}
	return 0;
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

static int name_from_dns(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family)
{
	unsigned char reply[1024] = { 0 }, *p = reply;
	char tmp[256];
	int i, cnt = 0;

	/* Perform one or more DNS queries for host */
	int result = __dns_query(reply, name, family, 0);
	if (result < 0) return result;

	for (i=0; i<result; i++) {
		if (family != AF_INET6) {
			int j = __dns_get_rr(&buf[cnt].addr, sizeof *buf, 4, MAXADDRS-cnt, p, RR_A, 0);
			while (j--) buf[cnt++].family = AF_INET;
		}
		if (family != AF_INET) {
			int j = __dns_get_rr(&buf[cnt].addr, sizeof *buf, 16, MAXADDRS-cnt, p, RR_AAAA, 0);
			while (j--) buf[cnt++].family = AF_INET6;
		}
		p += 512;
	}
	__dns_get_rr(tmp, 0, 256, 1, reply, RR_CNAME, 1);
	if (is_valid_hostname(tmp)) strcpy(canon, tmp);
	return cnt;
}

int __lookup_name(struct address buf[static MAXADDRS], char canon[static 256], const char *name, int family, int flags)
{
	int cnt = 0, i, j;

	*canon = 0;
	if (name) {
		size_t l;
		if ((l = strnlen(name, 256))-1 > 254)
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
	if (cnt<=0) cnt = name_from_numeric(buf, name, family);
	if (cnt<=0 && !(flags & AI_NUMERICHOST)) {
		cnt = name_from_hosts(buf, canon, name, family);
		if (cnt<=0) cnt = name_from_dns(buf, canon, name, family);
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
			buf[i].scopeid = 0;
			buf[i].family = AF_INET6;
		}
	}

	return cnt;
}
