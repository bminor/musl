#include <string.h>
#include <elf.h>

#define LDSO_ARCH "i386"

static int remap_rel(int type)
{
	switch(type) {
	case R_386_32:
		return REL_SYMBOLIC;
	case R_386_PC32:
		return REL_OFFSET;
	case R_386_GLOB_DAT:
		return REL_GOT;
	case R_386_JMP_SLOT:
		return REL_PLT;
	case R_386_RELATIVE:
		return REL_RELATIVE;
	case R_386_COPY:
		return REL_COPY;
	case R_386_TLS_DTPMOD32:
		return REL_DTPMOD;
	case R_386_TLS_DTPOFF32:
		return REL_DTPOFF;
	case R_386_TLS_TPOFF:
		return REL_TPOFF;
	case R_386_TLS_TPOFF32:
		return REL_TPOFF_NEG;
	case R_386_TLS_DESC:
		return REL_TLSDESC;
	}
	return 0;
}
