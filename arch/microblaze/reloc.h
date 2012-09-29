#include <string.h>
#include <elf.h>

#define ETC_LDSO_PATH "/etc/ld-musl-microblaze.path"

#define IS_COPY(x) ((x)==R_MICROBLAZE_COPY)
#define IS_PLT(x) ((x)==R_MICROBLAZE_JUMP_SLOT)

static inline void do_single_reloc(size_t *reloc_addr, int type, size_t sym_val, size_t sym_size, unsigned char *base_addr, size_t addend)
{
	switch(type) {
	case R_MICROBLAZE_32:
	case R_MICROBLAZE_GLOB_DAT:
	case R_MICROBLAZE_JUMP_SLOT:
		*reloc_addr = sym_val + addend;
		break;
	case R_MICROBLAZE_REL:
		*reloc_addr = (size_t)base_addr + addend;
		break;
	case R_MICROBLAZE_COPY:
		memcpy(reloc_addr, (void *)sym_val, sym_size);
		break;
	}
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
	if (!base) base = (char *)(t[AT_PHDR] & -4096);
	for (a=dynv; *a; a+=2) if (*a<20) t[*a] = a[1];
	n = t[DT_RELASZ];
	for (a=(void *)(base+t[DT_RELA]); n; a+=3, n-=12)
		if (a[1]%256 == R_MICROBLAZE_REL)
			*(size_t *)(base+a[0]) = (size_t)base + a[2];
}
