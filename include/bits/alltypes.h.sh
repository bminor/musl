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
TYPEDEF long ptrdiff_t;
TYPEDEF struct __va_list * va_list;

TYPEDEF long wchar_t;
TYPEDEF long wint_t;
TYPEDEF long wctrans_t;
TYPEDEF long wctype_t;

TYPEDEF char      int8_t;
TYPEDEF short     int16_t;
TYPEDEF int       int32_t;
TYPEDEF long long int64_t;

TYPEDEF unsigned char      uint8_t;
TYPEDEF unsigned short     uint16_t;
TYPEDEF unsigned int       uint32_t;
TYPEDEF unsigned long long uint64_t;

TYPEDEF unsigned char      __uint8_t;
TYPEDEF unsigned short     __uint16_t;
TYPEDEF unsigned int       __uint32_t;
TYPEDEF unsigned long long __uint64_t;

TYPEDEF int8_t  int_least8_t;
TYPEDEF int16_t int_least16_t;
TYPEDEF int32_t int_least32_t;
TYPEDEF int64_t int_least64_t;

TYPEDEF uint8_t  uint_least8_t;
TYPEDEF uint16_t uint_least16_t;
TYPEDEF uint32_t uint_least32_t;
TYPEDEF uint64_t uint_least64_t;

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

TYPEDEF long long          intmax_t;
TYPEDEF unsigned long long uintmax_t;

TYPEDEF long time_t;
TYPEDEF unsigned int useconds_t;
TYPEDEF int suseconds_t;
STRUCT timeval { time_t tv_sec; long tv_usec; };
STRUCT timespec { time_t tv_sec; long tv_nsec; };

TYPEDEF int pid_t;
TYPEDEF int id_t;
TYPEDEF int uid_t;
TYPEDEF int gid_t;
TYPEDEF int key_t;
TYPEDEF struct __pthread * pthread_t;

TYPEDEF long long off_t;

TYPEDEF unsigned int mode_t;

TYPEDEF unsigned int nlink_t;
TYPEDEF unsigned long long ino_t;
TYPEDEF long long dev_t;
TYPEDEF long blksize_t;
TYPEDEF long long blkcnt_t;
TYPEDEF unsigned long long fsblkcnt_t;
TYPEDEF unsigned long long fsfilcnt_t;

TYPEDEF int timer_t;
TYPEDEF int clockid_t;
TYPEDEF unsigned long clock_t;

TYPEDEF struct { unsigned long __bits[1024/sizeof(long)]; } sigset_t;
TYPEDEF struct __siginfo siginfo_t;

TYPEDEF unsigned int socklen_t;
TYPEDEF unsigned short sa_family_t;
TYPEDEF unsigned short in_port_t;
TYPEDEF unsigned int in_addr_t;
STRUCT in_addr { in_addr_t s_addr; };

TYPEDEF struct __FILE_s FILE;

TYPEDEF int nl_item;

TYPEDEF struct __locale * locale_t;


EOF
