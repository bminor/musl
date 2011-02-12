#define _GNU_SOURCE
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <inttypes.h>
#include <errno.h>
#include <string.h>

int getservbyport_r(int port, const char *prots,
	struct servent *se, char *buf, size_t buflen, struct servent **res)
{
	int i;
	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_port = port,
	};

	/* Align buffer */
	i = (uintptr_t)buf & sizeof(char *)-1;
	if (!i) i = sizeof(char *);
	if (buflen < 3*sizeof(char *)-i) {
		errno = ERANGE;
		return -1;
	}
	buf += sizeof(char *)-i;
	buflen -= sizeof(char *)-i;

	if (strcmp(prots, "tcp") && strcmp(prots, "udp")) return -1;

	se->s_port = port;
	se->s_proto = (char *)prots;
	se->s_aliases = (void *)buf;
	buf += 2*sizeof(char *);
	buflen -= 2*sizeof(char *);
	se->s_aliases[1] = 0;
	se->s_aliases[0] = se->s_name = buf;

	if (getnameinfo((void *)&sin, sizeof sin, 0, 0, buf, buflen, 
		strcmp(prots, "udp") ? 0 : NI_DGRAM) < 0) return -1;

	*res = se;
	return 0;
}
