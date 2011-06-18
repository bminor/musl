#include <string.h>
#include <elf.h>

#define IS_COPY(x) ((x)==R_386_COPY)

static inline void do_single_reloc(size_t *reloc_addr, int type, size_t sym_val, size_t sym_size, unsigned char *base_addr, size_t addend)
{
	switch(type) {
	case R_386_32:
		*reloc_addr += sym_val;
		break;
	case R_386_PC32:
		*reloc_addr += sym_val - (size_t)reloc_addr;
		break;
	case R_386_GLOB_DAT:
	case R_386_JMP_SLOT:
		*reloc_addr = sym_val;
		break;
	case R_386_RELATIVE:
		*reloc_addr += (size_t)base_addr;
		break;
	case R_386_COPY:
		memcpy(reloc_addr, (void *)sym_val, sym_size);
		break;
	}
}
