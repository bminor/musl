#define _GNU_SOURCE

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>

struct hostent *gethostbyname2(const char *name, int af)
{
	static struct hostent h;
	static long buf[512/sizeof(long)];
	struct hostent *res;
	if (gethostbyname2_r(name, af, &h,
		(void *)buf, sizeof buf, &res, &h_errno)) return 0;
	return &h;
}
