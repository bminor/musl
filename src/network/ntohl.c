#include <netinet/in.h>

uint32_t ntohl(uint32_t n)
{
	union {
		uint32_t i;
		uint8_t b[4];
	} u = { n };
	return (u.b[0]<<24) | (u.b[1]<<16) | (u.b[2]<<8) | u.b[3];
}
