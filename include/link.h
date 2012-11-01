#ifndef _LINK_H
#define _LINK_H

#include <elf.h>
#define __NEED_size_t
#include <bits/alltypes.h>

#if UINTPTR_MAX > 0xffffffff
#define ElfW(type) Elf64_ ## type
#else
#define ElfW(type) Elf32_ ## type
#endif

struct dl_phdr_info {
	ElfW(Addr) dlpi_addr;
	const char *dlpi_name;
	const ElfW(Phdr) *dlpi_phdr;
	ElfW(Half) dlpi_phnum;
	unsigned long long int dlpi_adds;
	unsigned long long int dlpi_subs;
	size_t dlpi_tls_modid;
	void *dlpi_tls_data;
};

int dl_iterate_phdr(int (*)(struct dl_phdr_info *, size_t, void *), void *);

#endif
