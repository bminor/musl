#include "pthread_impl.h"

void __unmapself_sh_mmu(void *, size_t);
void __unmapself_sh_nommu(void *, size_t);

#if !defined(__SH3__) && !defined(__SH4__)
#define __unmapself __unmapself_sh_nommu
#include "../../../src/thread/__unmapself.c"
#undef __unmapself
extern __attribute__((__visibility__("hidden"))) unsigned __sh_nommu;
#else
#define __sh_nommu 0
#endif

void __unmapself(void *base, size_t size)
{
	if (__sh_nommu) __unmapself_sh_nommu(base, size);
	else __unmapself_sh_mmu(base, size);
}
