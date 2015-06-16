#include "pthread_impl.h"
#include "libc.h"
#include "sh_atomic.h"
#include <elf.h>

/* Also perform sh-specific init */

#define CPU_HAS_LLSC 0x0040

__attribute__((__visibility__("hidden"))) unsigned __sh_atomic_model, __sh_nommu;

int __set_thread_area(void *p)
{
	size_t *aux;
	__asm__ __volatile__ ( "ldc %0, gbr" : : "r"(p) : "memory" );
#ifndef __SH4A__
	if (__hwcap & CPU_HAS_LLSC) {
		__sh_atomic_model = SH_A_LLSC;
		return 0;
	}
#if !defined(__SH3__) && !defined(__SH4__)
	for (aux=libc.auxv; *aux; aux+=2) {
		if (*aux != AT_PLATFORM) continue;
		const char *s = (void *)aux[1];
		if (s[0]!='s' || s[1]!='h' || s[2]!='2' || s[3]-'0'<10u) break;
		__sh_atomic_model = SH_A_IMASK;
		__sh_nommu = 1;
		return 0;
	}
#endif
	/* __sh_atomic_model = SH_A_GUSA; */ /* 0, default */
#endif
	return 0;
}
