#include <netinet/in.h>

uint32_t htonl(uint32_t n)
{
	union {
		uint8_t b[4];
		uint32_t i;
	} u = { { n>>24, n>>16, n>>8, n } };
	return u.i;
}
