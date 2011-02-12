#define _GNU_SOURCE

#include <netdb.h>
#include <string.h>
#include <netinet/in.h>

struct hostent *gethostbyaddr(const void *a, socklen_t l, int af)
{
	static struct hostent h;
	static long buf[512/sizeof(long)];
	struct hostent *res;
	if (gethostbyaddr_r(a, l, af, &h,
		(void *)buf, sizeof buf, &res, &h_errno)) return 0;
	return &h;
}
