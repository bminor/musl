#define MAP_FAILED ((void *) -1)

#define	PROT_NONE      0
#define	PROT_READ      1
#define	PROT_WRITE     2
#define	PROT_EXEC      4

#define	MAP_SHARED     0x01
#define	MAP_PRIVATE    0x02
#define	MAP_FIXED      0x10

/* linux extensions */
#define MAP_TYPE       0x0f
#define MAP_FILE       0x00
#define MAP_ANON       0x20
#define MAP_ANONYMOUS  MAP_ANON

#define MADV_NORMAL     0
#define MADV_RANDOM     1
#define MADV_SEQUENTIAL 2
#define MADV_WILLNEED   3
#define MADV_DONTNEED   4

#define POSIX_MADV_NORMAL       0
#define POSIX_MADV_RANDOM       1
#define POSIX_MADV_SEQUENTIAL   2
#define POSIX_MADV_WILLNEED     3
#define POSIX_MADV_DONTNEED     0

#define MS_ASYNC        1
#define MS_INVALIDATE   2
#define MS_SYNC         4

#define MCL_CURRENT     1
#define MCL_FUTURE      2

/* linux extensions */
#define MREMAP_MAYMOVE  1
#define MREMAP_FIXED    2
