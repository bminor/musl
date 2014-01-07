#include <sys/syscall.h>
long __syscall_cp_internal(volatile void*, long long, long long, long long, long long,
                             long long, long long, long long);

struct __timespec { long long tv_sec; long tv_nsec; };
struct __timespec_kernel { long long tv_sec; long long tv_nsec; };
#define __tsc(X) ((struct __timespec*)(unsigned long)(X))

long __syscall_cp_asm (volatile void * foo, long long n, long long a1, long long a2, long long a3,
	                     long long a4, long long a5, long long a6) {
	switch (n) {
	case SYS_mq_timedsend: case SYS_mq_timedreceive: case SYS_pselect6:
		if(a5) a5 = (unsigned long) (&(struct __timespec_kernel) {
		.tv_sec = __tsc(a5)->tv_sec, .tv_nsec = __tsc(a5)->tv_nsec});
		break;
	case SYS_futex:
		if(a4) a4 = (unsigned long) (&(struct __timespec_kernel) {
		.tv_sec = __tsc(a4)->tv_sec, .tv_nsec = __tsc(a4)->tv_nsec});
		if(n == SYS_futex) break;
	case SYS_clock_nanosleep:
	case SYS_rt_sigtimedwait: case SYS_ppoll:
		if(a3) a3 = (unsigned long) (&(struct __timespec_kernel) {
		.tv_sec = __tsc(a3)->tv_sec, .tv_nsec = __tsc(a3)->tv_nsec});
		break;
	case SYS_nanosleep:
		if(a1) a1 = (unsigned long) (&(struct __timespec_kernel) {
		.tv_sec = __tsc(a1)->tv_sec, .tv_nsec = __tsc(a1)->tv_nsec});
		if(a2) a2 = (unsigned long) (&(struct __timespec_kernel) {
		.tv_sec = __tsc(a2)->tv_sec, .tv_nsec = __tsc(a2)->tv_nsec});
		break;
	}
	return __syscall_cp_internal(foo, n, a1, a2, a3, a4, a5, a6);
}

