#!/bin/sh
sed -e << EOF \
'/^TYPEDEF/s/TYPEDEF \(.*\) \([^ ]*\);$/#if defined(__NEED_\2) \&\& !defined(__DEFINED_\2)\
typedef \1 \2;\
#define __DEFINED_\2\
#endif\
/
/^STRUCT/s/STRUCT * \([^ ]*\) \(.*\);$/#if defined(__NEED_struct_\1) \&\& !defined(__DEFINED_struct_\1)\
struct \1 \2;\
#define __DEFINED_struct_\1\
#endif\
/
/^UNION/s/UNION * \([^ ]*\) \(.*\);$/#if defined(__NEED_union_\1) \&\& !defined(__DEFINED_union_\1)\
union \1 \2;\
#define __DEFINED_union_\1\
#endif\
/'

TYPEDEF unsigned size_t;
TYPEDEF int ssize_t;
TYPEDEF int ptrdiff_t;

#if __GNUC__ >= 3
TYPEDEF __builtin_va_list va_list;
#else
TYPEDEF struct __va_list * va_list;
#endif

#ifndef __cplusplus
#ifdef __WCHAR_TYPE__
TYPEDEF __WCHAR_TYPE__ wchar_t;
#else
TYPEDEF long wchar_t;
#endif
#endif
TYPEDEF long wint_t;
TYPEDEF long wctrans_t;
TYPEDEF long wctype_t;

TYPEDEF signed char int8_t;
TYPEDEF short       int16_t;
TYPEDEF int         int32_t;
TYPEDEF long long   int64_t;

TYPEDEF unsigned char      uint8_t;
TYPEDEF unsigned short     uint16_t;
TYPEDEF unsigned int       uint32_t;
TYPEDEF unsigned long long uint64_t;

TYPEDEF unsigned short     __uint16_t;
TYPEDEF unsigned int       __uint32_t;
TYPEDEF unsigned long long __uint64_t;

TYPEDEF int8_t    int_fast8_t;
TYPEDEF int       int_fast16_t;
TYPEDEF int       int_fast32_t;
TYPEDEF int64_t   int_fast64_t;

TYPEDEF unsigned char      uint_fast8_t;
TYPEDEF unsigned int       uint_fast16_t;
TYPEDEF unsigned int       uint_fast32_t;
TYPEDEF uint64_t           uint_fast64_t;

TYPEDEF long          intptr_t;
TYPEDEF unsigned long uintptr_t;

#if defined(__FLT_EVAL_METHOD__) && __FLT_EVAL_METHOD__ == 0
TYPEDEF float float_t;
TYPEDEF double double_t;
#else
TYPEDEF long double float_t;
TYPEDEF long double double_t;
#endif

TYPEDEF long time_t;
TYPEDEF int suseconds_t;
STRUCT timeval { time_t tv_sec; int tv_usec; };
STRUCT timespec { time_t tv_sec; long tv_nsec; };

TYPEDEF int pid_t;
TYPEDEF int id_t;
TYPEDEF int uid_t;
TYPEDEF int gid_t;
TYPEDEF int key_t;

TYPEDEF struct __pthread * pthread_t;
TYPEDEF int pthread_once_t;
TYPEDEF int pthread_key_t;
TYPEDEF int pthread_spinlock_t;

TYPEDEF struct { union { int __i[9]; size_t __s[9]; } __u; } pthread_attr_t;
TYPEDEF unsigned pthread_mutexattr_t;
TYPEDEF unsigned pthread_condattr_t;
TYPEDEF unsigned pthread_barrierattr_t;
TYPEDEF struct { unsigned __attr[2]; } pthread_rwlockattr_t;

TYPEDEF struct { union { int __i[6]; void *__p[6]; } __u; } pthread_mutex_t;
TYPEDEF struct { union { int __i[12]; void *__p[12]; } __u; } pthread_cond_t;
TYPEDEF struct { union { int __i[8]; void *__p[8]; } __u; } pthread_rwlock_t;
TYPEDEF struct { union { int __i[5]; void *__p[5]; } __u; } pthread_barrier_t;

TYPEDEF long long off_t;

TYPEDEF unsigned int mode_t;

TYPEDEF unsigned int nlink_t;
TYPEDEF unsigned long long ino_t;
TYPEDEF long long dev_t;
TYPEDEF long blksize_t;
TYPEDEF long long blkcnt_t;
TYPEDEF unsigned long long fsblkcnt_t;
TYPEDEF unsigned long long fsfilcnt_t;

TYPEDEF void * timer_t;
TYPEDEF int clockid_t;
TYPEDEF unsigned long clock_t;

TYPEDEF struct { unsigned long __bits[128/sizeof(long)]; } sigset_t;
TYPEDEF struct __siginfo siginfo_t;

TYPEDEF unsigned int socklen_t;
TYPEDEF unsigned short sa_family_t;
TYPEDEF unsigned short in_port_t;
TYPEDEF unsigned int in_addr_t;
STRUCT in_addr { in_addr_t s_addr; };

TYPEDEF struct __FILE_s FILE;

TYPEDEF int nl_item;

TYPEDEF struct __locale * locale_t;

STRUCT iovec { void *iov_base; size_t iov_len; };

EOF
