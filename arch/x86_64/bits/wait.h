#define WNOHANG    1
#define WUNTRACED  2

#define WSTOPPED   2
#define WEXITED    4
#define WCONTINUED 8
#define WNOWAIT    0x1000000

#if defined(__XOPEN_SOURCE) || defined(__GNU_SOURCE)
#define P_ALL  0
#define P_PID  1
#define P_PGID 2
#endif
