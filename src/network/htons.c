#include <netinet/in.h>

uint16_t htons(uint16_t n)
{
	union {
		uint8_t b[2];
		uint16_t s;
	} u = { { n>>8, n } };
	return u.s;
}
