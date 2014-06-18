#include <stdint.h>
#include <string.h>
#include <elf.h>

#define LDSO_ARCH "x32"

/* FIXME: x32 is very strange in its use of 64-bit relocation types in
 * a 32-bit environment. As long as the memory at reloc_addr is
 * zero-filled prior to relocations, just treating 64-bit relocations
 * as operating on 32-bit slots should be fine, but this should be
 * checked. In particular, R_X86_64_64, R_X86_64_DTPOFF64, and
 * R_X86_64_TPOFF64 may need checking. */

static int remap_rel(int type)
{
	switch(type) {
	case R_X86_64_64:
	case R_X86_64_32:
		return REL_SYMBOLIC;
	case R_X86_64_PC32:
		return REL_OFFSET;
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
	case R_X86_64_DTPOFF32:
		return REL_DTPOFF;
	case R_X86_64_TPOFF64:
	case R_X86_64_TPOFF32:
		return REL_TPOFF;
	}
	return 0;
}
