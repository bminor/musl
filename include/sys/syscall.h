#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H
#ifdef __cplusplus
extern "C" {
#endif

long __syscall_ret(unsigned long);
long __syscall(long, ...);
long syscall(long, ...);

#include <bits/syscall.h>

#define __syscall1(n,a) __syscall1(n,(long)(a))
#define __syscall2(n,a,b) __syscall2(n,(long)(a),(long)(b))
#define __syscall3(n,a,b,c) __syscall3(n,(long)(a),(long)(b),(long)(c))
#define __syscall4(n,a,b,c,d) __syscall4(n,(long)(a),(long)(b),(long)(c),(long)(d))
#define __syscall5(n,a,b,c,d,e) __syscall5(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e))
#define __syscall6(n,a,b,c,d,e,f) __syscall6(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),(long)(f))

#define __SYSCALL_NARGS_X(a,b,c,d,e,f,g,n,...) n
#define __SYSCALL_NARGS(...) __SYSCALL_NARGS_X(__VA_ARGS__,6,5,4,3,2,1,0)
#define __SYSCALL_CONCAT_X(a,b) a##b
#define __SYSCALL_CONCAT(a,b) __SYSCALL_CONCAT_X(a,b)
#define __SYSCALL_DISP(b,...) __SYSCALL_CONCAT(b,__SYSCALL_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define __syscall(...) __SYSCALL_DISP(__syscall,__VA_ARGS__)
#define syscall(...) __syscall_ret(__syscall(__VA_ARGS__))

#ifdef __cplusplus
}
#endif
#endif
