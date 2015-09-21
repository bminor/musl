#include <stddef.h>
#include "dynlink.h"

#ifdef SHARED

#ifndef START
#define START "_dlstart"
#endif

#include "crt_arch.h"

#ifndef GETFUNCSYM
#define GETFUNCSYM(fp, sym, got) do { \
	__attribute__((__visibility__("hidden"))) void sym(); \
	static void (*static_func_ptr)() = sym; \
	__asm__ __volatile__ ( "" : "+m"(static_func_ptr) : : "memory"); \
	*(fp) = static_func_ptr; } while(0)
#endif

__attribute__((__visibility__("hidden")))
void _dlstart_c(size_t *sp, size_t *dynv)
{
	size_t i, aux[AUX_CNT], dyn[DYN_CNT];
	size_t *rel, rel_size, base;

	int argc = *sp;
	char **argv = (void *)(sp+1);

	for (i=argc+1; argv[i]; i++);
	size_t *auxv = (void *)(argv+i+1);

	for (i=0; i<AUX_CNT; i++) aux[i] = 0;
	for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT)
		aux[auxv[i]] = auxv[i+1];

	for (i=0; i<DYN_CNT; i++) dyn[i] = 0;
	for (i=0; dynv[i]; i+=2) if (dynv[i]<DYN_CNT)
		dyn[dynv[i]] = dynv[i+1];

	/* If the dynamic linker is invoked as a command, its load
	 * address is not available in the aux vector. Instead, compute
	 * the load address as the difference between &_DYNAMIC and the
	 * virtual address in the PT_DYNAMIC program header. */
	base = aux[AT_BASE];
	if (!base) {
		size_t phnum = aux[AT_PHNUM];
		size_t phentsize = aux[AT_PHENT];
		Phdr *ph = (void *)aux[AT_PHDR];
		for (i=phnum; i--; ph = (void *)((char *)ph + phentsize)) {
			if (ph->p_type == PT_DYNAMIC) {
				base = (size_t)dynv - ph->p_vaddr;
				break;
			}
		}
	}

	/* MIPS uses an ugly packed form for GOT relocations. Since we
	 * can't make function calls yet and the code is tiny anyway,
	 * it's simply inlined here. */
	if (NEED_MIPS_GOT_RELOCS) {
		size_t local_cnt = 0;
		size_t *got = (void *)(base + dyn[DT_PLTGOT]);
		for (i=0; dynv[i]; i+=2) if (dynv[i]==DT_MIPS_LOCAL_GOTNO)
			local_cnt = dynv[i+1];
		for (i=0; i<local_cnt; i++) got[i] += base;
	}

	rel = (void *)(base+dyn[DT_REL]);
	rel_size = dyn[DT_RELSZ];
	for (; rel_size; rel+=2, rel_size-=2*sizeof(size_t)) {
		if (!IS_RELATIVE(rel[1])) continue;
		size_t *rel_addr = (void *)(base + rel[0]);
		*rel_addr += base;
	}

	rel = (void *)(base+dyn[DT_RELA]);
	rel_size = dyn[DT_RELASZ];
	for (; rel_size; rel+=3, rel_size-=3*sizeof(size_t)) {
		if (!IS_RELATIVE(rel[1])) continue;
		size_t *rel_addr = (void *)(base + rel[0]);
		*rel_addr = base + rel[2];
	}

	stage2_func dls2;
	GETFUNCSYM(&dls2, __dls2, base+dyn[DT_PLTGOT]);
	dls2((void *)base, sp);
}

#endif
