#ifndef	_SETJMP_H
#define	_SETJMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bits/setjmp.h>
typedef unsigned long sigjmp_buf[(1024+sizeof(jmp_buf))/sizeof(long)];

#ifdef _GNU_SOURCE
#define jmp_buf sigjmp_buf
#endif

int setjmp (jmp_buf);
int _setjmp (jmp_buf);
int sigsetjmp (sigjmp_buf, int);

void longjmp (jmp_buf, int);
void _longjmp (jmp_buf, int);
void siglongjmp (sigjmp_buf, int);

#define setjmp setjmp
#define longjmp longjmp

#ifdef __cplusplus
}
#endif

#endif
