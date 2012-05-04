#ifndef _BYTESWAP_H
#define _BYTESWAP_H

#include <stdint.h>

#if __STDC_VERSION__ >= 199901L
inline
#endif
static uint16_t __bswap_16(uint16_t __x)
{
	return __x<<8 | __x>>8;
}

#if __STDC_VERSION__ >= 199901L
inline
#endif
static uint32_t __bswap_32(uint32_t __x)
{
	return __x>>24 | __x>>8&0xff00 | __x<<8&0xff0000 | __x<<24;
}

#if __STDC_VERSION__ >= 199901L
inline
#endif
static uint64_t __bswap_64(uint64_t __x)
{
	return __bswap_32(__x)+0ULL<<32 | __bswap_32(__x>>32);
}

#define bswap_16(x) __bswap_16(x)
#define bswap_32(x) __bswap_32(x)
#define bswap_64(x) __bswap_64(x)

#endif
