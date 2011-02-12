#include <endian.h>
#include <stdint.h>

uint32_t bswap_32(uint32_t);

uint64_t bswap_64(uint64_t x)
{
	return bswap_32(x)+0LL<<32 | bswap_32(x>>32);
}
