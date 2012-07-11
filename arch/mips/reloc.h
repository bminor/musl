#include <string.h>
#include <elf.h>

#define ETC_LDSO_PATH "/etc/ld-musl-mips.path"

#define IS_COPY(x) ((x)==R_MIPS_COPY)
#define IS_PLT(x) ((x)==R_MIPS_JUMP_SLOT)

static inline void do_single_reloc(size_t *reloc_addr, int type, size_t sym_val, size_t sym_size, unsigned char *base_addr, size_t addend)
{
	switch(type) {
	case R_MIPS_JUMP_SLOT:
		*reloc_addr = sym_val;
		break;
	case R_MIPS_REL32:
		// FIXME: how do symbolic relocs come in here ?
		*reloc_addr += (size_t)base_addr;
		break;
	case R_MIPS_COPY:
		memcpy(reloc_addr, (void *)sym_val, sym_size);
		break;
	}
}
