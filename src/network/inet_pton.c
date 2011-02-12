#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "__dns.h"

int inet_pton(int af, const char *s, void *a0)
{
	unsigned char *a = a0;
	const char *z;
	unsigned long x;
	int i;

	/* Reimplement this because inet_pton cannot accept special v4 forms */
	if (af==AF_INET) {
		for (i=0; i<4 && *s; i++) {
			a[i] = x = strtoul(s, (char **)&z, 10);
			if (!isdigit(*s) || z==s || (*z && *z != '.') || x>255)
				return 0;
			s=z+1;
		}
		return 0;
	} else if (af==AF_INET6) {
		return !__ipparse(a, AF_INET6, s);
	}

	errno = EAFNOSUPPORT;
	return 0;
}
