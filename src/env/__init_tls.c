#include <elf.h>
#include <limits.h>
#include "pthread_impl.h"
#include "libc.h"
#include "atomic.h"

#ifndef SHARED

static void *image;
static size_t len, bss, align;

void *__copy_tls(unsigned char *mem, size_t cnt)
{
	mem += ((uintptr_t)image - (uintptr_t)mem) & (align-1);
	memcpy(mem, image, len);
	return mem + len + bss;
}

static void *simple(void *p)
{
	*(void **)p = p;
	return __set_thread_area(p) ? 0 : p;
}

weak_alias(simple, __install_initial_tls);

void *__mmap(void *, size_t, int, int, int, off_t);

#if ULONG_MAX == 0xffffffff
typedef Elf32_Phdr Phdr;
#else
typedef Elf64_Phdr Phdr;
#endif

#define AUX_CNT 6

void __init_tls(size_t *auxv)
{
	size_t i, aux[AUX_CNT] = { 0 };
	unsigned char *p, *mem;
	size_t n, d;
	Phdr *phdr, *tls_phdr=0;
	size_t base = 0;

	for (; auxv[0]; auxv+=2) if (auxv[0]<AUX_CNT) aux[auxv[0]] = auxv[1];
	p = (void *)aux[AT_PHDR];
	for (p=(void *)aux[AT_PHDR]; aux[AT_PHNUM]--; p+=aux[AT_PHENT]) {
		phdr = (void *)p;
		if (phdr->p_type == PT_PHDR)
			base = aux[AT_PHDR] - phdr->p_vaddr;
		if (phdr->p_type == PT_TLS)
			tls_phdr = phdr;
	}
	if (!tls_phdr) return;

	libc.tls_size = len+bss+align+4*sizeof(size_t)+sizeof(struct pthread);

	image = (void *)(base + tls_phdr->p_vaddr);
	len = tls_phdr->p_filesz;
	bss = tls_phdr->p_memsz - len;
	align = tls_phdr->p_align;
	if (align < 4*sizeof(size_t)) align = 4*sizeof(size_t);
	mem = __mmap(0, libc.tls_size, PROT_READ|PROT_WRITE,
		MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
	if (mem == MAP_FAILED) a_crash();

	if (!__install_initial_tls(__copy_tls(mem, 0))) a_crash();
}
#else
void __init_tls(size_t *auxv) { }
#endif
