#include <resolv.h>
#include <string.h>

int res_querydomain(const char *name, const char *domain, int class, int type, unsigned char *dest, int len)
{
	char tmp[256];
	size_t nl = strnlen(name, 256);
	size_t dl = strnlen(domain, 256);
	if (nl+dl+1 > 255) return -1;
	memcpy(tmp, name, nl);
	tmp[nl] = '.';
	memcpy(tmp+nl+1, domain, dl+1);
	return res_query(tmp, class, type, dest, len);
}
