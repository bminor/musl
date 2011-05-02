#ifndef _SYSCALL_H
#define _SYSCALL_H

/* This header is mostly useless leftover wrapper cruft */

#include <sys/syscall.h>

#define socketcall __socketcall
#define socketcall_cp __socketcall_cp

#define __syscall_cp0(n) (__syscall_cp)(n,0,0,0,0,0,0)
#define __syscall_cp1(n,a) (__syscall_cp)(n,(long)(a),0,0,0,0,0)
#define __syscall_cp2(n,a,b) (__syscall_cp)(n,(long)(a),(long)(b),0,0,0,0)
#define __syscall_cp3(n,a,b,c) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),0,0,0)
#define __syscall_cp4(n,a,b,c,d) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),(long)(d),0,0)
#define __syscall_cp5(n,a,b,c,d,e) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),0)
#define __syscall_cp6(n,a,b,c,d,e,f) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),(long)(f))

long (__syscall_cp)(long, long, long, long, long, long, long);

#define __syscall_cp(...) __SYSCALL_DISP(__syscall_cp,__VA_ARGS__)
#define syscall_cp(...) __syscall_ret(__syscall_cp(__VA_ARGS__))

#endif
