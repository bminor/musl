#ifndef _ENDIAN_H
#define _ENDIAN_H

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __PDP_ENDIAN 3412

#include <bits/endian.h>

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define BIG_ENDIAN __BIG_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#define PDP_ENDIAN __PDP_ENDIAN
#define BYTE_ORDER __BYTE_ORDER
#endif

#endif
