#include <string.h>
#include <elf.h>
#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ENDIAN_SUFFIX "el"
#else
#define ENDIAN_SUFFIX ""
#endif

#define LDSO_ARCH "microblaze" ENDIAN_SUFFIX

#define TPOFF_K 0

static int remap_rel(int type)
{
	switch(type) {
	case R_MICROBLAZE_32:
		return REL_SYMBOLIC;
	case R_MICROBLAZE_GLOB_DAT:
		return REL_GOT;
	case R_MICROBLAZE_JUMP_SLOT:
		return REL_PLT;
	case R_MICROBLAZE_REL:
		return REL_RELATIVE;
	case R_MICROBLAZE_COPY:
		return REL_COPY;
	case R_MICROBLAZE_TLSDTPMOD32:
		return REL_DTPMOD;
	case R_MICROBLAZE_TLSDTPREL32:
		return REL_DTPOFF;
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
		if (a[1]%256 == R_MICROBLAZE_REL)
			*(size_t *)(base+a[0]) = (size_t)base + a[2];
}
