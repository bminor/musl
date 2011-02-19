#ifndef _UCONTEXT_H
#define _UCONTEXT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>

#ifdef _GNU_SOURCE
#define ucontext __ucontext
#endif

int  getcontext(ucontext_t *);
void makecontext(ucontext_t *, void (*)(void), int, ...);
int  setcontext(const ucontext_t *);
int  swapcontext(ucontext_t *, const ucontext_t *);

#ifdef __cplusplus
}
#endif
#endif
