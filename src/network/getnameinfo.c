#include <netdb.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int __dns_parse(const unsigned char *, int, int (*)(void *, int, const void *, int, const void *), void *);
int __dn_expand(const unsigned char *, const unsigned char *, const unsigned char *, char *, int);
int __res_mkquery(int, const char *, int, int, const unsigned char *, int, const unsigned char*, unsigned char *, int);
int __res_send(const unsigned char *, int, unsigned char *, int);

#define PTR_MAX (64 + sizeof ".in-addr.arpa")
#define RR_PTR 12

static void mkptr4(char *s, const unsigned char *ip)
{
	sprintf(s, "%d.%d.%d.%d.in-addr.arpa",
		ip[3], ip[2], ip[1], ip[0]);
}

static void mkptr6(char *s, const unsigned char *ip)
{
	static const char xdigits[] = "0123456789abcdef";
	int i;
	for (i=15; i>=0; i--) {
		*s++ = xdigits[ip[i]&15]; *s++ = '.';
		*s++ = xdigits[ip[i]>>4]; *s++ = '.';
	}
	strcpy(s, "ip6.arpa");
}

static int dns_parse_callback(void *c, int rr, const void *data, int len, const void *packet)
{
	char tmp[256];
	if (rr != RR_PTR) return 0;
	if (__dn_expand(packet, (const unsigned char *)packet + 512,
	    data, tmp, sizeof tmp) > 0)
		strcpy(c, tmp);
	return 0;
	
}

int getnameinfo(const struct sockaddr *restrict sa, socklen_t sl,
	char *restrict node, socklen_t nodelen,
	char *restrict serv, socklen_t servlen,
	int flags)
{
	char ptr[PTR_MAX];
	char buf[256];
	int af = sa->sa_family;
	unsigned char *a;

	switch (af) {
	case AF_INET:
		a = (void *)&((struct sockaddr_in *)sa)->sin_addr;
		if (sl != sizeof(struct sockaddr_in)) return EAI_FAMILY;
		mkptr4(ptr, a);
		break;
	case AF_INET6:
		a = (void *)&((struct sockaddr_in6 *)sa)->sin6_addr;
		if (sl != sizeof(struct sockaddr_in6)) return EAI_FAMILY;
		if (memcmp(a, "\0\0\0\0\0\0\0\0\0\0\xff\xff", 12))
			mkptr6(ptr, a);
		else
			mkptr4(ptr, a+12);
		break;
	default:
		return EAI_FAMILY;
	}

	if (node && nodelen) {
		buf[0] = 0;
		if (!(flags & NI_NUMERICHOST)) {
			unsigned char query[18+PTR_MAX], reply[512];
			int qlen = __res_mkquery(0, ptr, 1, RR_PTR,
				0, 0, 0, query, sizeof query);
			int rlen = __res_send(query, qlen, reply, sizeof reply);
			buf[0] = 0;
			if (rlen > 0)
				__dns_parse(reply, rlen, dns_parse_callback, buf);
		}
		if (!*buf) {
			if (flags & NI_NAMEREQD) return EAI_NONAME;
			inet_ntop(af, a, buf, sizeof buf);
		}
		if (strlen(buf) >= nodelen) return EAI_OVERFLOW;
		strcpy(node, buf);
	}

	if (serv && servlen) {
		if (snprintf(buf, sizeof buf, "%d",
			ntohs(((struct sockaddr_in *)sa)->sin_port))>=servlen)
			return EAI_OVERFLOW;
		strcpy(serv, buf);
	}

	return 0;
}
