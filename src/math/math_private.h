/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifndef _MATH_PRIVATE_H_
#define _MATH_PRIVATE_H_

#include <inttypes.h>

/*
 * The original fdlibm code used statements like:
 *      n0 = ((*(int*)&one)>>29)^1;             * index of high word *
 *      ix0 = *(n0+(int*)&x);                   * high word of x *
 *      ix1 = *((1-n0)+(int*)&x);               * low word of x *
 * to dig two 32 bit words out of the 64 bit IEEE floating point
 * value.  That is non-ANSI, and, moreover, the gcc instruction
 * scheduler gets it wrong.  We instead use the following macros.
 * Unlike the original code, we determine the endianness at compile
 * time, not at run time; I don't see much benefit to selecting
 * endianness at run time.
 */

/*
 * A union which permits us to convert between a double and two 32 bit
 * ints.
 */

typedef union
{
  double value;
  uint64_t words;
} ieee_double_shape_type;

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)                                \
do {                                                            \
  ieee_double_shape_type ew_u;                                  \
  ew_u.value = (d);                                             \
  (ix0) = ew_u.words >> 32;                                     \
  (ix1) = (uint32_t)ew_u.words;                                 \
} while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)                                      \
do {                                                            \
  ieee_double_shape_type gh_u;                                  \
  gh_u.value = (d);                                             \
  (i) = gh_u.words >> 32;                                       \
} while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)                                       \
do {                                                            \
  ieee_double_shape_type gl_u;                                  \
  gl_u.value = (d);                                             \
  (i) = (uint32_t)gl_u.words;                                   \
} while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)                                 \
do {                                                            \
  ieee_double_shape_type iw_u;                                  \
  iw_u.words = ((uint64_t)(ix0) << 32) | (ix1);                 \
  (d) = iw_u.value;                                             \
} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)                                      \
do {                                                            \
  ieee_double_shape_type sh_u;                                  \
  sh_u.value = (d);                                             \
  sh_u.words &= 0xffffffff;                                     \
  sh_u.words |= ((uint64_t)(v) << 32);                          \
  (d) = sh_u.value;                                             \
} while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)                                       \
do {                                                            \
  ieee_double_shape_type sl_u;                                  \
  sl_u.value = (d);                                             \
  sl_u.words &= 0xffffffff00000000ull;                          \
  sl_u.words |= (uint32_t)(v);                                  \
  (d) = sl_u.value;                                             \
} while (0)

/*
 * A union which permits us to convert between a float and a 32 bit
 * int.
 */

typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)                                     \
do {                                                            \
  ieee_float_shape_type gf_u;                                   \
  gf_u.value = (d);                                             \
  (i) = gf_u.word;                                              \
} while (0)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)                                     \
do {                                                            \
  ieee_float_shape_type sf_u;                                   \
  sf_u.word = (i);                                              \
  (d) = sf_u.value;                                             \
} while (0)

/* fdlibm kernel function */
int     __ieee754_rem_pio2(double,double*);
double  __kernel_sin(double,double,int);
double  __kernel_cos(double,double);
double  __kernel_tan(double,double,int);
int     __kernel_rem_pio2(double*,double*,int,int,int,const int*);

/* float versions of fdlibm kernel functions */
int     __ieee754_rem_pio2f(float,float*);
float   __kernel_sinf(float,float,int);
float   __kernel_cosf(float,float);
float   __kernel_tanf(float,float,int);
int     __kernel_rem_pio2f(float*,float*,int,int,int,const int*);

#endif /* !_MATH_PRIVATE_H_ */
