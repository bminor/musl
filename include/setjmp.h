#ifndef	_SETJMP_H
#define	_SETJMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bits/setjmp.h>


#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
 || defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)
typedef unsigned long sigjmp_buf[(128+sizeof(jmp_buf))/sizeof(long)];
#ifdef _GNU_SOURCE
#define jmp_buf sigjmp_buf
#endif
int sigsetjmp (sigjmp_buf, int);
void siglongjmp (sigjmp_buf, int);
#endif


#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE)
int _setjmp (jmp_buf);
void _longjmp (jmp_buf, int);
#endif


int setjmp (jmp_buf);
void longjmp (jmp_buf, int);

#define setjmp setjmp
#define longjmp longjmp

#ifdef __cplusplus
}
#endif

#endif
