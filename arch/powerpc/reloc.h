#include <string.h>
#include <elf.h>

#define LDSO_ARCH "powerpc"

#define TPOFF_K (-0x7000)

static int remap_rel(int type)
{
	switch(type) {
	case R_PPC_ADDR32:
		return REL_SYMBOLIC;
	case R_PPC_GLOB_DAT:
		return REL_GOT;
	case R_PPC_JMP_SLOT:
		return REL_PLT;
	case R_PPC_RELATIVE:
		return REL_RELATIVE;
	case R_PPC_COPY:
		return REL_COPY;
	case R_PPC_DTPMOD32:
		return REL_DTPMOD;
	case R_PPC_DTPREL32:
		return REL_DTPOFF;
	case R_PPC_TPREL32:
		return REL_TPOFF;
	}
	return 0;
}

void __reloc_self(int c, size_t *a, size_t *dynv)
{
	char *base;
	size_t t[20], n;
	for (a+=c+1; *a; a++);
	for (a++; *a; a+=2) if (*a<20) t[*a] = a[1];
	base = (char *)t[AT_BASE];
	if (!base) base = (char *)(t[AT_PHDR] & -t[AT_PAGESZ]);
	for (a=dynv; *a; a+=2) if (*a<20) t[*a] = a[1];
	n = t[DT_RELASZ];
	for (a=(void *)(base+t[DT_RELA]); n; a+=3, n-=12)
		if (a[1]%256 == R_PPC_RELATIVE)
			*(size_t *)(base+a[0]) = (size_t)base + a[2];
}
