#ifndef _UCONTEXT_H
#define _UCONTEXT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>

struct __fpstate {
	unsigned long __x[7];
	unsigned char __y[80];
	unsigned long __z;
};

typedef struct {
	unsigned long __gregs[19];
	void *__fpregs;
	unsigned long __oldmask, __cr2;
} mcontext_t;

typedef struct __ucontext {
	unsigned long uc_flags;
	struct __ucontext *uc_link;
	stack_t uc_stack;
	mcontext_t uc_mcontext;
	sigset_t uc_sigmask;
	struct __fpstate __fpregs_mem;
} ucontext_t;

int  getcontext(ucontext_t *);
void makecontext(ucontext_t *, void (*)(void), int, ...);
int  setcontext(const ucontext_t *);
int  swapcontext(ucontext_t *, const ucontext_t *);

#ifdef __cplusplus
}
#endif
#endif
