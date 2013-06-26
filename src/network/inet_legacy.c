#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "__dns.h"

in_addr_t inet_addr(const char *p)
{
	struct sockaddr_in sin;
	if (__ipparse(&sin, AF_INET, p)) return -1;
	return sin.sin_addr.s_addr;
}

in_addr_t inet_network(const char *p)
{
	return ntohl(inet_addr(p));
}

int inet_aton(const char *cp, struct in_addr *inp)
{
	return inet_pton(AF_INET, cp, (void *)inp) > 0;
}

char *inet_ntoa(struct in_addr in)
{
	static char buf[16];
	unsigned char *a = (void *)&in;
	snprintf(buf, sizeof buf, "%d.%d.%d.%d", a[0], a[1], a[2], a[3]);
	return buf;
}

struct in_addr inet_makeaddr(int net, int host)
{
	uint32_t n = net, h = host;
	if (n < 256) h |= n<<24;
	else if (n < 65536) h |= n<<16;
	else h |= n<<8;
	return (struct in_addr){ h };
}

in_addr_t inet_lnaof(struct in_addr in)
{
	uint32_t h = in.s_addr;
	if (h>>24 < 128) return h & 0xffffff;
	if (h>>24 < 192) return h & 0xffff;
	return h & 0xff;
}

in_addr_t inet_netof(struct in_addr in)
{
	uint32_t h = in.s_addr;
	if (h>>24 < 128) return h >> 24;
	if (h>>24 < 192) return h >> 16;
	return h >> 8;
}
