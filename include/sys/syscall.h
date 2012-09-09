#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H
#ifdef __cplusplus
extern "C" {
#endif

#include <features.h>

long syscall(long, ...);

#include <bits/syscall.h>

#ifdef __cplusplus
}
#endif
#endif
