#include <endian.h>
#include <stdint.h>

uint32_t bswap_32(uint32_t x)
{
	return x>>24 | x>>8&0xff00 | x<<8&0xff0000 | x<<24;
}
