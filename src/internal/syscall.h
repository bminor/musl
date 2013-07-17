#ifndef _INTERNAL_SYSCALL_H
#define _INTERNAL_SYSCALL_H

#if defined(__PIC__) && (100*__GNUC__+__GNUC_MINOR__ >= 303)
__attribute__((visibility("protected")))
#endif
long __syscall_ret(unsigned long), __syscall(long, ...),
	__syscall_cp(long, long, long, long, long, long, long);

#include <sys/syscall.h>
#include "syscall_arch.h"

#define __syscall1(n,a) __syscall1(n,(long)(a))
#define __syscall2(n,a,b) __syscall2(n,(long)(a),(long)(b))
#define __syscall3(n,a,b,c) __syscall3(n,(long)(a),(long)(b),(long)(c))
#define __syscall4(n,a,b,c,d) __syscall4(n,(long)(a),(long)(b),(long)(c),(long)(d))
#define __syscall5(n,a,b,c,d,e) __syscall5(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e))
#define __syscall6(n,a,b,c,d,e,f) __syscall6(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),(long)(f))
#define __syscall7(n,a,b,c,d,e,f,g) (__syscall)(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),(long)(f),(long)g)

#define __SYSCALL_NARGS_X(a,b,c,d,e,f,g,h,n,...) n
#define __SYSCALL_NARGS(...) __SYSCALL_NARGS_X(__VA_ARGS__,7,6,5,4,3,2,1,0,)
#define __SYSCALL_CONCAT_X(a,b) a##b
#define __SYSCALL_CONCAT(a,b) __SYSCALL_CONCAT_X(a,b)
#define __SYSCALL_DISP(b,...) __SYSCALL_CONCAT(b,__SYSCALL_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define __syscall(...) __SYSCALL_DISP(__syscall,__VA_ARGS__)
#define syscall(...) __syscall_ret(__syscall(__VA_ARGS__))

#define socketcall __socketcall
#define socketcall_cp __socketcall_cp

#define __syscall_cp0(n) (__syscall_cp)(n,0,0,0,0,0,0)
#define __syscall_cp1(n,a) (__syscall_cp)(n,(long)(a),0,0,0,0,0)
#define __syscall_cp2(n,a,b) (__syscall_cp)(n,(long)(a),(long)(b),0,0,0,0)
#define __syscall_cp3(n,a,b,c) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),0,0,0)
#define __syscall_cp4(n,a,b,c,d) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),(long)(d),0,0)
#define __syscall_cp5(n,a,b,c,d,e) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),0)
#define __syscall_cp6(n,a,b,c,d,e,f) (__syscall_cp)(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),(long)(f))

#define __syscall_cp(...) __SYSCALL_DISP(__syscall_cp,__VA_ARGS__)
#define syscall_cp(...) __syscall_ret(__syscall_cp(__VA_ARGS__))

#ifdef SYS_socket
#define __socketcall(nm,a,b,c,d,e,f) syscall(SYS_##nm, a, b, c, d, e, f)
#define __socketcall_cp(nm,a,b,c,d,e,f) syscall_cp(SYS_##nm, a, b, c, d, e, f)
#else
#define __socketcall(nm,a,b,c,d,e,f) syscall(SYS_socketcall, __SC_##nm, \
    ((long [6]){ (long)a, (long)b, (long)c, (long)d, (long)e, (long)f }))
#define __socketcall_cp(nm,a,b,c,d,e,f) syscall_cp(SYS_socketcall, __SC_##nm, \
    ((long [6]){ (long)a, (long)b, (long)c, (long)d, (long)e, (long)f }))
#endif

/* fixup legacy 16-bit junk */

#ifdef SYS_getuid32
#undef SYS_lchown
#undef SYS_getuid
#undef SYS_getgid
#undef SYS_geteuid
#undef SYS_getegid
#undef SYS_setreuid
#undef SYS_setregid
#undef SYS_getgroups
#undef SYS_setgroups
#undef SYS_fchown
#undef SYS_setresuid
#undef SYS_getresuid
#undef SYS_setresgid
#undef SYS_getresgid
#undef SYS_chown
#undef SYS_setuid
#undef SYS_setgid
#undef SYS_setfsuid
#undef SYS_setfsgid
#define SYS_lchown SYS_lchown32
#define SYS_getuid SYS_getuid32
#define SYS_getgid SYS_getgid32
#define SYS_geteuid SYS_geteuid32
#define SYS_getegid SYS_getegid32
#define SYS_setreuid SYS_setreuid32
#define SYS_setregid SYS_setregid32
#define SYS_getgroups SYS_getgroups32
#define SYS_setgroups SYS_setgroups32
#define SYS_fchown SYS_fchown32
#define SYS_setresuid SYS_setresuid32
#define SYS_getresuid SYS_getresuid32
#define SYS_setresgid SYS_setresgid32
#define SYS_getresgid SYS_getresgid32
#define SYS_chown SYS_chown32
#define SYS_setuid SYS_setuid32
#define SYS_setgid SYS_setgid32
#define SYS_setfsuid SYS_setfsuid32
#define SYS_setfsgid SYS_setfsgid32
#endif


/* fixup legacy 32-bit-vs-lfs64 junk */

#ifdef SYS_fcntl64
#undef SYS_fcntl
#define SYS_fcntl SYS_fcntl64
#endif

#ifdef SYS_getdents64
#undef SYS_getdents
#define SYS_getdents SYS_getdents64
#endif

#ifdef SYS_ftruncate64
#undef SYS_ftruncate
#undef SYS_truncate
#define SYS_ftruncate SYS_ftruncate64
#define SYS_truncate SYS_truncate64
#endif

#ifdef SYS_stat64
#undef SYS_stat
#undef SYS_fstat
#undef SYS_lstat
#undef SYS_statfs
#undef SYS_fstatfs
#define SYS_stat SYS_stat64
#define SYS_fstat SYS_fstat64
#define SYS_lstat SYS_lstat64
#define SYS_statfs SYS_statfs64
#define SYS_fstatfs SYS_fstatfs64
#endif

#if defined(SYS_newfstatat)
#undef SYS_fstatat
#define SYS_fstatat SYS_newfstatat
#elif defined(SYS_fstatat64)
#undef SYS_fstatat
#define SYS_fstatat SYS_fstatat64
#endif

#ifdef SYS_ugetrlimit
#undef SYS_getrlimit
#define SYS_getrlimit SYS_ugetrlimit
#endif

#ifdef SYS__newselect
#undef SYS_select
#define SYS_select SYS__newselect
#endif

#ifdef SYS_pread64
#undef SYS_pread
#undef SYS_pwrite
#define SYS_pread SYS_pread64
#define SYS_pwrite SYS_pwrite64
#endif

#ifdef SYS_fadvise64_64
#undef SYS_fadvise
#define SYS_fadvise SYS_fadvise64_64
#endif

#endif
