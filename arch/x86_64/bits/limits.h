#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)
#define PIPE_BUF 4096
#define PAGESIZE 4096
#define PAGE_SIZE PAGESIZE
#define FILESIZEBITS 64
#define NAME_MAX 255
#define SYMLINK_MAX 255
#define PATH_MAX 4096
#define NZERO 20
#define NGROUPS_MAX 32
#define ARG_MAX 131072
#define IOV_MAX 1024
#define SYMLOOP_MAX 40
#define WORD_BIT 64
#define LONG_BIT 64
#endif

#define SHRT_MIN  (-1-0x7fff)
#define SHRT_MAX  0x7fff
#define USHRT_MAX 0xffff

#define INT_MIN  (-1-0x7fffffff)
#define INT_MAX  0x7fffffff
#define UINT_MAX 0xffffffffU

#define LONG_MIN  (-1-0x7fffffffffffffffL)
#define LONG_MAX  0x7fffffffffffffffL
#define ULONG_MAX 0xffffffffffffffffUL

#define LLONG_MIN  (-1-0x7fffffffffffffffLL)
#define LLONG_MAX  0x7fffffffffffffffLL
#define ULLONG_MAX 0xffffffffffffffffULL

