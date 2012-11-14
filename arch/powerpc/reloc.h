#include <string.h>
#include <elf.h>

#define ETC_LDSO_PATH "/etc/ld-musl-powerpc.path"

#define IS_COPY(x) ((x)==R_PPC_COPY)
#define IS_PLT(x) ((x)==R_PPC_JMP_SLOT)

// see linux' arch/powerpc/include/asm/elf.h 
static inline void do_single_reloc(
	struct dso *self, unsigned char *base_addr,
	size_t *reloc_addr, int type, size_t addend,
	Sym *sym, size_t sym_size,
	struct symdef def, size_t sym_val)
{
	switch(type) {
	case R_PPC_GLOB_DAT:
	case R_PPC_JMP_SLOT:
		*reloc_addr = sym_val;
		break;
	case R_PPC_REL32:
		if (sym_val) *reloc_addr += sym_val;
		else *reloc_addr += (size_t)base_addr;
		break;
	case R_PPC_COPY:
		memcpy(reloc_addr, (void *)sym_val, sym_size);
		break;
	case R_PPC_RELATIVE:
		*reloc_addr += (size_t)base_addr;
		break;
	//case R_PPC64_DTPMOD64:  //R_X86_64_DTPMOD64:
	case R_PPC_DTPMOD32:  //R_386_TLS_DTPMOD32:
		*reloc_addr = def.dso ? def.dso->tls_id : self->tls_id;
		break;
	}
}
