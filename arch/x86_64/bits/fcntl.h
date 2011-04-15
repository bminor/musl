#define O_CREAT        0100
#define O_EXCL         0200
#define O_NOCTTY       0400
#define O_TRUNC       01000
#define O_APPEND      02000
#define O_NONBLOCK    04000
#define O_SYNC       010000
#define O_DIRECTORY 0200000
#define O_NOFOLLOW  0400000
#define O_CLOEXEC  02000000

/* Extensions, but in the reserved namespace, so OK */
#define O_ASYNC      020000
#define O_DIRECT     040000
#define O_LARGEFILE       0
#define O_NOATIME  01000000
#define O_NDELAY O_NONBLOCK
#define F_DUPFD_CLOEXEC 1030

#ifdef _GNU_SOURCE
#define FAPPENT O_APPEND
#define FFSYNC O_FSYNC
#define FASYNC O_ASYNC
#define FNONBLOCK O_NONBLOCK
#define FNDELAY O_NDELAY
#endif

#define F_DUPFD  0
#define F_GETFD  1
#define F_SETFD  2
#define F_GETFL  3
#define F_SETFL  4

#define F_SETOWN 8
#define F_GETOWN 9

#define F_GETLK 5
#define F_SETLK 6
#define F_SETLKW 7
