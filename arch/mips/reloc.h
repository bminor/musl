#include <string.h>
#include <elf.h>
#include <endian.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ENDIAN_SUFFIX "el"
#else
#define ENDIAN_SUFFIX ""
#endif

#ifdef __mips_soft_float
#define FP_SUFFIX "-sf"
#else
#define FP_SUFFIX ""
#endif

#define LDSO_ARCH "mips" ENDIAN_SUFFIX FP_SUFFIX

#define TPOFF_K (-0x7000)

static int remap_rel(int type)
{
	switch(type) {
	case R_MIPS_REL32:
		return REL_SYM_OR_REL;
	case R_MIPS_JUMP_SLOT:
		return REL_PLT;
	case R_MIPS_COPY:
		return REL_COPY;
	case R_MIPS_TLS_DTPMOD32:
		return REL_DTPMOD;
	case R_MIPS_TLS_DTPREL32:
		return REL_DTPOFF;
	case R_MIPS_TLS_TPREL32:
		return REL_TPOFF;
	}
	return 0;
}

void __reloc_self(int c, size_t *a, size_t *dynv, size_t *got)
{
	char *base;
	size_t t[20], n;
	for (a+=c+1; *a; a++);
	for (a++; *a; a+=2) if (*a<20) t[*a] = a[1];
	base = (char *)t[AT_BASE];
	if (!base) base = (char *)(t[AT_PHDR] & -t[AT_PAGESZ]);
	for (a=dynv; *a; a+=2) if (*a-0x70000000UL<20) t[*a&31] = a[1];
	n = t[DT_MIPS_LOCAL_GOTNO - 0x70000000];
	for (a=got; n; a++, n--) *a += (size_t)base;
}

static void do_relocs(struct dso *dso, size_t *rel, size_t rel_size, size_t stride);

static void do_arch_relocs(struct dso *this, struct dso *head)
{
	unsigned char *base = this->base;
	size_t *dynv = this->dynv;
	size_t dyn[20] = {0};
	size_t i;
	size_t rel[2], got=0;
	Sym *sym;

	for (i=0; dynv[i]; i+=2) {
		if (dynv[i]-0x70000000UL<20)
			dyn[dynv[i]&31] = dynv[i+1];
		else if (dynv[i] == DT_PLTGOT)
			got = dynv[i+1];
	}
	i = dyn[DT_MIPS_LOCAL_GOTNO-0x70000000];
	if (this->shortname && !strcmp(this->shortname, "libc.so")) {
		got += sizeof(size_t) * i;
	} else {
		for (; i; i--, got+=sizeof(size_t))
			*(size_t *)(base+got) += (size_t)base;
	}
	sym = this->syms + dyn[DT_MIPS_GOTSYM-0x70000000];
	i = dyn[DT_MIPS_SYMTABNO-0x70000000] - dyn[DT_MIPS_GOTSYM-0x70000000];
	for (; i; i--, got+=sizeof(size_t), sym++) {
		rel[0] = got;
		rel[1] = sym-this->syms << 8 | R_MIPS_JUMP_SLOT;
		*(size_t *)(base+got) = 0;
		do_relocs(this, rel, sizeof rel, 2);
	}
}

#define NEED_ARCH_RELOCS 1
#define DYNAMIC_IS_RO 1
#define ARCH_SYM_REJECT_UND(s) (!((s)->st_other & STO_MIPS_PLT))
