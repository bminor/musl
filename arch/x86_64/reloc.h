#include <stdint.h>
#include <string.h>
#include <elf.h>

#define LDSO_ARCH "x86_64"

static int remap_rel(int type)
{
	switch(type) {
	case R_X86_64_64:
		return REL_SYMBOLIC;
	case R_X86_64_PC32:
		return REL_OFFSET32;
	case R_X86_64_GLOB_DAT:
		return REL_GOT;
	case R_X86_64_JUMP_SLOT:
		return REL_PLT;
	case R_X86_64_RELATIVE:
		return REL_RELATIVE;
	case R_X86_64_COPY:
		return REL_COPY;
	case R_X86_64_DTPMOD64:
		return REL_DTPMOD;
	case R_X86_64_DTPOFF64:
		return REL_DTPOFF;
	case R_X86_64_TPOFF64:
		return REL_TPOFF;
	case R_X86_64_TLSDESC:
		return REL_TLSDESC;
	}
	return 0;
}
