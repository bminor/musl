#include <string.h>
#include <elf.h>

#define ETC_LDSO_PATH "/etc/ld-musl-arm.path"

#define IS_COPY(x) ((x)==R_ARM_COPY)
#define IS_PLT(x) ((x)==R_ARM_JUMP_SLOT)

static inline void do_single_reloc(size_t *reloc_addr, int type, size_t sym_val, size_t sym_size, unsigned char *base_addr, size_t addend)
{
	switch(type) {
	case R_ARM_ABS32:
		*reloc_addr += sym_val;
		break;
	case R_ARM_GLOB_DAT:
	case R_ARM_JUMP_SLOT:
		*reloc_addr = sym_val;
		break;
	case R_ARM_RELATIVE:
		*reloc_addr += (size_t)base_addr;
		break;
	case R_ARM_COPY:
		memcpy(reloc_addr, (void *)sym_val, sym_size);
		break;
	}
}
