#ifndef _STDINT_H
#define _STDINT_H

#define __NEED_int8_t
#define __NEED_int16_t
#define __NEED_int32_t
#define __NEED_int64_t

#define __NEED_uint8_t
#define __NEED_uint16_t
#define __NEED_uint32_t
#define __NEED_uint64_t

#define __NEED_int_least8_t
#define __NEED_int_least16_t
#define __NEED_int_least32_t
#define __NEED_int_least64_t

#define __NEED_uint_least8_t
#define __NEED_uint_least16_t
#define __NEED_uint_least32_t
#define __NEED_uint_least64_t

#define __NEED_int_fast8_t
#define __NEED_int_fast16_t
#define __NEED_int_fast32_t
#define __NEED_int_fast64_t

#define __NEED_uint_fast8_t
#define __NEED_uint_fast16_t
#define __NEED_uint_fast32_t
#define __NEED_uint_fast64_t

#define __NEED_intptr_t
#define __NEED_uintptr_t
#define __NEED_intmax_t
#define __NEED_uintmax_t

#include <bits/alltypes.h>

#if !defined __cplusplus || defined __STDC_LIMIT_MACROS

#define INT8_MIN   (-1-0x7f)
#define INT16_MIN  (-1-0x7fff)
#define INT32_MIN  (-1-0x7fffffff)
#define INT64_MIN  (-1-0x7fffffffffffffffLL)

#define INT8_MAX   (0x7f)
#define INT16_MAX  (0x7fff)
#define INT32_MAX  (0x7fffffff)
#define INT64_MAX  (0x7fffffffffffffffLL)

#define UINT8_MAX  (0xff)
#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffff)
#define UINT64_MAX (0xffffffffffffffffULL)

#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST64_MIN  INT64_MIN

#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MAX  INT64_MAX

#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#undef WCHAR_MIN
#undef WCHAR_MAX
#undef WINT_MIN
#undef WINT_MAX
#define WCHAR_MIN INT32_MIN
#define WCHAR_MAX INT32_MAX
#define WINT_MIN INT32_MIN
#define WINT_MAX INT32_MAX

#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#include <bits/stdint.h>

#endif

#if !defined __cplusplus || defined __STDC_CONSTANT_MACROS

#define INT8_C(c)  c
#define INT16_C(c) c
#define INT32_C(c) c
#define INT64_C(c) c ## LL

#define UINT8_C(c)  c ## U
#define UINT16_C(c) c ## U
#define UINT32_C(c) c ## U
#define UINT64_C(c) c ## ULL

#define INTMAX_C(c)  c ## LL
#define UINTMAX_C(c) c ## ULL

#endif

#endif
