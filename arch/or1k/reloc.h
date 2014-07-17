#include <string.h>
#include <elf.h>
#include <endian.h>

#define LDSO_ARCH "or1k"

#define TPOFF_K 0

static int remap_rel(int type)
{
	switch(type) {
	case R_OR1K_32:
		return REL_SYMBOLIC;
	case R_OR1K_GLOB_DAT:
		return REL_GOT;
	case R_OR1K_JMP_SLOT:
		return REL_PLT;
	case R_OR1K_RELATIVE:
		return REL_RELATIVE;
	case R_OR1K_COPY:
		return REL_COPY;
	case R_OR1K_TLS_DTPMOD:
		return REL_DTPMOD;
	case R_OR1K_TLS_DTPOFF:
		return REL_DTPOFF;
	case R_OR1K_TLS_TPOFF:
		return REL_TPOFF;
	}
	return 0;
}

#include "syscall.h"
void __reloc_self(int c, size_t *a, size_t *dynv)
{
	char dot = '.', ex = 'x';
	char *base;
	size_t t[20], n;
	for (a+=c+1; *a; a++);
	for (a++; *a; a+=2) if (*a<20) t[*a] = a[1];
	base = (char *)t[AT_BASE];
	if (!base) base = (char *)(t[AT_PHDR] & -t[AT_PAGESZ]);
	for (a=dynv; *a; a+=2) if (*a<20) t[*a] = a[1];
	n = t[DT_RELASZ];
	for (a=(void *)(base+t[DT_RELA]); n; a+=3, n-=12)
		if (a[1]%256 == R_OR1K_RELATIVE)
			*(size_t *)(base+a[0]) = (size_t)base + a[2];
}
