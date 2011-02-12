#include <netinet/in.h>

uint16_t ntohs(uint16_t n)
{
	union {
		uint16_t s;
		uint8_t b[2];
	} u = { n };
	return (u.b[0]<<8) | u.b[1];
}
