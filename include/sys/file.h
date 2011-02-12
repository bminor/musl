#ifndef _SYS_FILE_H
#define _SYS_FILE_H
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

#define LOCK_SH	1
#define LOCK_EX	2
#define LOCK_NB	4
#define LOCK_UN	8

extern int flock(int, int);

#endif

#ifdef __cplusplus
}
#endif
#endif
