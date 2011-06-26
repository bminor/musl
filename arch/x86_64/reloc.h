#include <stdint.h>
#include <string.h>
#include <elf.h>

#define ETC_LDSO_PATH "/etc/ld-musl-x86_64.path"

#define IS_COPY(x) ((x)==R_X86_64_COPY)
#define IS_PLT(x) ((x)==R_X86_64_JUMP_SLOT)

static inline void do_single_reloc(size_t *reloc_addr, int type, size_t sym_val, size_t sym_size, unsigned char *base_addr, size_t addend)
{
	switch(type) {
	case R_X86_64_GLOB_DAT:
	case R_X86_64_JUMP_SLOT:
	case R_X86_64_64:
		*reloc_addr = sym_val + addend;
		break;
	case R_X86_64_32:
		*(uint32_t *)reloc_addr = sym_val + addend;
		break;
	case R_X86_64_PC32:
		*reloc_addr = sym_val + addend - (size_t)reloc_addr + (size_t)base_addr;
		break;
	case R_X86_64_RELATIVE:
		*reloc_addr = (size_t)base_addr + addend;
		break;
	case R_X86_64_COPY:
		memcpy(reloc_addr, (void *)sym_val, sym_size);
		break;
	}
}
